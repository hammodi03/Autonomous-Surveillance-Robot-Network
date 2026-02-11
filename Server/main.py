import threading
import asyncio
import time
from view import MapApp
import tkinter as tk
import app_log
from app_log import log

PORT = 5000
#HOST = "127.0.0.1"

devices = {}
vo_commands = {}  # device_id -> (action, vo_id)
battery_commands = {}
kill_commands = set()
revive_commands = set()
teleport_commands = {}
add_person_commands = {}
remove_person_commands = []
person_list = {}
personID = 0
obstacle_commands = {}   # device_id -> bool
radar_commands = {}      # device_id -> bool

map_app = None

async def handle_client(reader, writer):
    addr = writer.get_extra_info("peername")

    try:
        while True:
            try:
                data = await reader.read(1024)
            except (OSError, ConnectionAbortedError) as e:
                log(f"Connection error from {addr}: {e}")
                break
            if not data:
                break

            msg = data.decode().strip()

            try:
                id_str, x_str, y_str, goalX_str, goalY_str, battery_str, state_str, alarm_str = msg.split(",")
            # parts = msg.split(",")
            # if len(parts) == 7:
                #     id_str, x_str, y_str, goalX_str, goalY_str, battery_str, state_str = parts
                #     alarm_str = "False"
                # elif len(parts) == 8:
                #     id_str, x_str, y_str, goalX_str, goalY_str, battery_str, state_str, alarm_str = parts
                # else:
                #     raise ValueError(f"Expected 7 or 8 fields, got {len(parts)}")


                device_id = int(id_str)
                x = float(x_str)
                y = float(y_str)
                goalX = float(goalX_str)
                goalY = float(goalY_str)
                battery = float(battery_str)
                alarm = str(alarm_str).strip().lower() == "true"
                # Initiera device om den saknas
                if device_id not in devices:
                    devices[device_id] = {"pos": (0, 0), "goal": (0, 0), "battery": 0, "state": "null", "alarm": False}
                    if map_app is not None:
                        map_app.redraw_rooms_only()

                devices[device_id]["pos"] = (x, y)
                devices[device_id]["goal"] = (goalX, goalY)
                devices[device_id]["battery"] = battery
                devices[device_id]["state"] = state_str
                devices[device_id]["alarm"] = alarm
                devices[device_id]["last_seen"] = time.time()

                log(f"Agent id: {device_id}. X: {x}, Y: {y}, Goal X: {goalX}, Goal Y: {goalY}, Battery: {battery}. State: {state_str}. Alarm: {alarm}")

            except Exception as e:
                log(f"Fel format från {addr}: {msg} ({e})")
                continue

            global vo_commands, battery_commands
            # -----------------------BATTERY CHANGE---------------------------
            if device_id in battery_commands:
                new_batt = battery_commands.pop(device_id)
                cmd = f"BATTERY_SET:{device_id}:{new_batt}\n"
                log(f"Skickar ny batterinivå till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
                await writer.drain()
            # -----------------------VALUABLE OBJECTS---------------------------
            elif device_id in vo_commands:
                action, vo_id = vo_commands.pop(device_id) # Targeted to THIS robot

                vo_data = devices.get(f"ValObj{vo_id}", {})
                room = vo_data.get("room", -1)
                real_id = vo_data.get("id", -1)

                if action == "STOLEN":
                    cmd = f"VO_STOLEN:{vo_id}:{real_id}:{room}\n"
                elif action == "RETURNED":
                    cmd = f"VO_RETURNED:{vo_id}:{real_id}:{room}\n"
                else:
                    cmd = "OK\n"

                log(f"Skickar VO-kommando till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
                await writer.drain() # Added missing drain
            # -----------------------KILL---------------------------
            elif device_id in kill_commands:
                kill_commands.remove(device_id)
                cmd = f"KILL_DEVICE:{device_id}\n"
                log(f"Skickar KILL till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
                await writer.drain()
            # -----------------------REVIVE---------------------------
            elif device_id in revive_commands:
                revive_commands.remove(device_id)
                cmd = f"REVIVE_DEVICE:{device_id}\n"
                log(f"Skickar REVIVE till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
                await writer.drain()
            # -----------------------TELEPORT---------------------------
            elif device_id in teleport_commands:
                tx, ty, room = teleport_commands.pop(device_id)
                cmd = f"TELEPORT:{device_id}:{tx}:{ty}:{room}\n"
                log(f"Skickar TELEPORT till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
                await writer.drain()
            # -----------------------ACTIVATE OBSTACLE DETECTORS---------------------------
            elif device_id in obstacle_commands:
                enabled = obstacle_commands.pop(device_id)
                cmd = f"OBSTACLE_DETECTORS:{device_id}:{1 if enabled else 0}\n"
                log(f"Skickar OBSTACLE_DETECTORS till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
                await writer.drain()
            # -----------------------ACTIVATE RADAR---------------------------
            elif device_id in radar_commands:
                enabled = radar_commands.pop(device_id)
                cmd = f"RADAR:{device_id}:{1 if enabled else 0}\n"
                log(f"Skickar RADAR till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
                await writer.drain()
            # -----------------------ADD PERSON---------------------------
            elif device_id in add_person_commands:
                pX, pY, pRoom, pID = add_person_commands.pop(device_id)
                cmd = f"ADD_PERSON:{device_id}:{pX}:{pY}:{pRoom}: {pID}\n"
                log(f"Skickar ADD_PERSON till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())
            # -----------------------REMOVE PERSON---------------------------
            elif device_id in remove_person_commands:
                pID = add_person_commands.pop(device_id)
                cmd = f"REMOVE_PERSON:{pID}\n"
                log(f"Skickar REMOVE_PERSON till klient {device_id}: {cmd.strip()}")
                writer.write(cmd.encode())

            else:
                writer.write(b"OK\n")

                # writer.write(b"ACK\n")
                # await writer.drain()

    finally:
        writer.close()
        await writer.wait_closed()

def check_disconnected():
    now = time.time()
    
    for k, v in list(devices.items()):
        if not isinstance(k, int):
            continue
        
        last = v.get("last_seen", 0)
        elapsed = now - last
        
        if elapsed > 3:
            if v.get("state") != "Disconnected":
                v["state"] = "Disconnected"
                v["alarm"] = False
                v["disconnected_since"] = now
        
        if v.get("state") == "Disconnected":
            since = v.get("disconnected_since", now)
            if now - since > 5:
                log(f"Device {k} removed after disconnect timeout")
                del devices[k]
                
                if map_app is not None:  # Better check than globals()
                    map_app.redraw_rooms_only()






async def start_server():
    server = await asyncio.start_server(handle_client, "0.0.0.0", PORT)
    log(f"Server started on port {PORT}. Waiting for ESP32 connections...")
    async with server:
        await server.serve_forever()


def run_server():
    def checker():
        while True:
            check_disconnected()
            time.sleep(1)

    threading.Thread(target=checker, daemon=True).start()

    asyncio.run(start_server())

def place_valuable_object():
    def walker1():
        name = 10

            #ROOM 1
            #x_start, x_end = 5, 10.5
            #y_start, y_end = 49, 67

            #ROOM 11
        x_start, x_end = 3, 49.5
        y_start, y_end = 13, 18.5

        x = x_start
        y = y_start
        battery = 99.6
        state_str = "Moving"

        devices["ValObj1"] = {"pos": (6.5, 66), "id": 1, "room": 1}
        devices["ValObj2"] = {"pos": (7.5, 55), "id": 2, "room": 1}

        devices["ValObj3"] = {"pos": (7.5, 25), "id": 2, "room": 3}
        devices["ValObj4"] = {"pos": (6.5, 36), "id": 1, "room": 3}

        devices["ValObj5"] = {"pos": (40, 27), "id": 1, "room": 9}
            # devices["ValObj100"] = {"pos": (26.5, 27), "id": 1, "room": 6} # For testing ZONES
            # devices["ValObj101"] = {"pos": (30, 27), "id": 1, "room": 7} # For testing ZONES
            # devices["ValObj102"] = {"pos": (35, 27), "id": 1, "room": 8} # For testing ZONES
        devices["ValObj6"] = {"pos": (22, 27), "id": 1, "room": 5}

        devices["ValObj7"] = {"pos": (30, 16), "id": 1, "room": 11}
        devices["ValObj8"] = {"pos": (4, 16), "id": 2, "room": 11}

        devices["ValObj9"] = {"pos": (47, 6), "id": 1, "room": 15}
        devices["ValObj10"] = {"pos": (53, 5), "id": 1, "room": 16}

        devices["ValObj11"] = {"pos": (58, 16), "id": 1, "room": 12}
        devices["ValObj12"] = {"pos": (50, 24), "id": 1, "room": 13}
        devices["ValObj13"] = {"pos": (48, 40), "id": 1, "room": 14}

        devices["Charger1"] = {"pos": (5.5, 45.5), "room": 2}
        devices["Charger2"] = {"pos": (45.5, 16), "room": 11}
        devices["Charger3"] = {"pos": (19, 22), "room": 4}
        devices["Charger4"] = {"pos": (54.5, 28), "room": 13}
        
        devices["LabelRobot"] = {"pos": (53, 65.5)}
        devices["LabelValObj"]  = {"pos": (53, 64)}
        devices["LabelCharger"] = {"pos": (53, 62.5)}


        # devices[name] = {"pos": (x, y), "battery": battery, "state": state_str}

        # step = 0.1
        # delay = 0.05
        # while True:
        #     while y < y_end:
        #         y = min(y + step, y_end)
        #         devices[name]["pos"] = (x, y)
        #         devices[name]["battery"] = battery
        #         devices[name]["state"] = state_str
        #         time.sleep(delay)
        #     battery -= 1

        #     while x < x_end:
        #         x = min(x + step, x_end)
        #         devices[name]["pos"] = (x, y)
        #         devices[name]["battery"] = battery
        #         devices[name]["state"] = state_str
        #         time.sleep(delay)
        #     battery -= 1

        #     while y > y_start:
        #         y = max(y - step, y_start)
        #         devices[name]["pos"] = (x, y)
        #         devices[name]["battery"] = battery
        #         devices[name]["state"] = state_str
        #         time.sleep(delay)
        #     battery -= 1

        #     while x > x_start:
        #         x = max(x - step, x_start)
        #         devices[name]["pos"] = (x, y)
        #         devices[name]["battery"] = battery
        #         devices[name]["state"] = state_str
        #         time.sleep(delay)
        #     battery -= 1

    threading.Thread(target=walker1, daemon=True).start()


def run_view():
    root = tk.Tk()
    root.title("Map View")
    app_log.init_log_window(root)
    app = MapApp(root, devices, vo_commands, battery_commands, kill_commands, revive_commands, teleport_commands, add_person_commands, remove_person_commands, person_list, obstacle_commands, radar_commands)

    place_valuable_object()

    root.mainloop()


if __name__ == "__main__":
    # Starta view i bakgrundstråd
    threading.Thread(target=run_view, daemon=True).start()

    # Starta server i huvudtråd
    run_server()