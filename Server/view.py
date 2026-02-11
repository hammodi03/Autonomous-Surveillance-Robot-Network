import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk
from app_log import log
import time

class MapApp:
    ESP_X_MIN = 4.5
    ESP_X_MAX = 62
    ESP_Y_MIN = 0.5
    ESP_Y_MAX = 67.5

    PY_X_MIN = 160
    PY_X_MAX = 1990
    PY_Y_MIN = 80
    PY_Y_MAX = 2135


    currID = 0

    def __init__(self, root, devices, vo_commands, battery_commands, kill_commands, revive_commands, teleport_commands, add_person_commands, remove_person_commands, person_list, obstacle_commands, radar_commands):
        self.root = root
        self.devices = devices
        self.vo_commands = vo_commands
        self.battery_commands = battery_commands
        self.kill_commands = kill_commands
        self.revive_commands = revive_commands
        self.teleport_commands = teleport_commands
        self.add_person_commands = add_person_commands
        self.remove_person_commands = remove_person_commands
        self.person_list = person_list
        self.obstacle_commands = obstacle_commands
        self.radar_commands = radar_commands
        self.obstacle_enabled = False
        self.radar_enabled = False
        self.show_info_boxes = True
        self.show_rooms = True


        self.rooms = [
            (1, 4.5, 11, 48, 67.5),
            (2, 3.5, 11, 42, 48),
            (3, 4.5, 17, 19, 42),
            (4, 17, 42.5, 20.8, 29),
            (5, 20.3, 24, 23.5, 29),
            (6, 24.5, 28.3, 23.5, 29),
            (7, 28.7, 32.8, 23.5, 29),
            (8, 33.8, 37, 23.5, 29),
            (9, 37.3, 42.5, 23.5, 29),
            (10, 42.5, 45, 20.8, 29),
            (11, 1.5, 50.8, 12.5, 19),
            (12, 50.8, 62, 9.8, 19.8),
            (13, 45.5, 58.5, 19.8, 30.2),
            (14, 45.7, 54.5, 30.2, 42.5),
            (15, 44.5, 49.8, 0, 11.3),
            (16, 49.8, 58.5, 0.5, 9.8),
        ]



        self.points = {}
        self.selected_vos = set()
        self.original_image = Image.open("resources/map.png")
        self.image_width, self.image_height = self.original_image.size

        # Create Main Container
        main_frame = tk.Frame(root)
        main_frame.pack(fill="both", expand=True)

        # 1. CREATE THE FRAMES FIRST
        map_frame = tk.Frame(main_frame)
        map_frame.pack(side="left", fill="both", expand=True)

        info_frame = tk.Frame(main_frame, width=320)
        info_frame.pack(side="right", fill="y")
        info_frame.pack_propagate(False)

        # 2. CREATE CANVAS INSIDE map_frame
        self.canvas = tk.Canvas(map_frame)
        self.canvas.pack(fill="both", expand=True)

        # 3. CREATE UI ELEMENTS INSIDE info_frame
        self.add_person_btn = ttk.Button(
            info_frame, 
            text="➕ Add Person to Simulation", 
            command=self.open_add_person_dialog
        )
        self.add_person_btn.pack(fill="x", padx=10, pady=5)

        self.toggle_info_btn = ttk.Button(
            info_frame,
            text="Hide info boxes",
            command=self.toggle_info_boxes
        )
        self.toggle_info_btn.pack(fill="x", padx=10, pady=5)

        self.toggle_rooms_btn = ttk.Button(
            info_frame,
            text="Hide room boundaries",
            command=self.toggle_rooms
        )
        self.toggle_rooms_btn.pack(fill="x", padx=10, pady=5)



        info_title = tk.Label(
            info_frame,
            text="Room information for teleporting",
            font=("Calibri", 14, "bold")
        )
        info_title.pack(anchor="w", padx=10, pady=(10, 5))

        self.room_text = tk.Text(
            info_frame,
            wrap="none",
            font=("Consolas", 11),
            height=40,
            width=40
        )
        self.room_text.pack(fill="both", expand=True, padx=10, pady=(0, 10))
        self.room_text.configure(state="disabled")

        # Initialization
        self.canvas.bind("<Button-1>", self.on_click)
        self.root.bind("<Configure>", self.on_resize)

        self.scale = 1.0
        self.redraw()
        self.update_loop()
        self.update_room_info()

    

    def remove_person(self, incomingID):
        # 1. Remove from the local list/object
        # Note: Ensure self.person_list supports this check or use its specific methods
        if incomingID in self.person_list: 
            self.person_list.pop(incomingID)
            log(f"Removing Person {incomingID} from local list.")
    
            # 2. Queue the REMOVE command for all active robots
            for dev_id in self.devices:
                if isinstance(dev_id, int):
                    # Use self. to access the list stored during __init__
                    self.remove_person_commands.append(incomingID) 
        else:
            log(f"Warning: Attempted to remove non-existent Person ID {incomingID}")
        

    def open_add_person_dialog(self):
        win = tk.Toplevel(self.root)
        win.title("Add Person")
        win.geometry("300x250")
        win.attributes("-topmost", True)

        frame = ttk.Frame(win, padding=20)
        frame.pack(fill="both", expand=True)

        ttk.Label(frame, text="X Coordinate:").grid(row=0, column=0, sticky="w", pady=5)
        x_var = tk.StringVar()
        ttk.Entry(frame, textvariable=x_var).grid(row=0, column=1, pady=5)

        ttk.Label(frame, text="Y Coordinate:").grid(row=1, column=0, sticky="w", pady=5)
        y_var = tk.StringVar()
        ttk.Entry(frame, textvariable=y_var).grid(row=1, column=1, pady=5)

        ttk.Label(frame, text="Room Number:").grid(row=2, column=0, sticky="w", pady=5)
        room_var = tk.StringVar()
        ttk.Entry(frame, textvariable=room_var).grid(row=2, column=1, pady=5)

        def submit():
            try:
                px = float(x_var.get())
                py = float(y_var.get())
                pRoom = int(room_var.get())
        
                MapApp.currID += 1
                current_id = MapApp.currID
        
                # CORRECT: Save person data to the dictionary
                self.person_list[current_id] = {'x': px, 'y': py, 'room': pRoom}
        
                # CORRECT: If add_person_commands is a list, use .append
                for dev_id in self.devices:
                    if isinstance(dev_id, int):
                        # Vi använder dev_id som nyckel
                        self.add_person_commands[dev_id] = (px, py, pRoom, current_id)
                        log(f"Added Person {current_id} for Robot {dev_id}")
                win.destroy()
            except ValueError:
                log("Error: Please enter valid numbers for Person data.")

        ttk.Button(frame, text="Add to All Robots", command=submit).grid(row=3, column=0, columnspan=2, pady=20)

    # Rest of the methods (on_click, redraw, etc.) remain as you had them...

    def on_click(self, event):
        sx, sy = event.x, event.y
        closest_device = None
        min_dist = 20

        for addr, info in self.devices.items():
            x_esp, y_esp = info["pos"]
            px, py = self.esp_to_py(x_esp, y_esp)
            px *= self.scale
            py *= self.scale
            dist = ((px - sx) ** 2 + (py - sy) ** 2) ** 0.5
            if dist < min_dist:
                closest_device = addr
                min_dist = dist
        self.selected_device = closest_device
        if isinstance(self.selected_device, int):
            self.open_device_popup(self.selected_device)
            return
        if not self.selected_device:
            return

        # ValObj-logik
        if "ValObj" in str(self.selected_device):
            try:
                vo_id = int(str(self.selected_device).replace("ValObj", ""))
            except ValueError:
                log(f"Kunde inte tolka VO-id från {self.selected_device}")
                return

            vo_key = f"ValObj{vo_id}"
            vo_data = self.devices.get(vo_key, {})
            room = vo_data.get("room", "-")
            real_id = vo_data.get("id", "-")

            # Determine action
            if vo_id in self.selected_vos:
                self.selected_vos.remove(vo_id)
                action = "RETURNED"
                log(f"Returned ID: {real_id} to room: {room}")
            else:
                self.selected_vos.add(vo_id)
                action = "STOLEN"
                log(f"Stole ID: {real_id} from room: {room}")

            # DISPATCH TO ALL ROBOTS
            for dev_id in self.devices:
                if isinstance(dev_id, int):
                    # Store as a dictionary entry for each robot
                    self.vo_commands[dev_id] = (action, vo_id)

            self.draw_all_points()
            return

        # Övriga enheter
        if self.selected_device:
            x, y = self.devices[self.selected_device]["pos"]
            self.x_entry.delete(0, tk.END)
            self.x_entry.insert(0, str(x))
            self.y_entry.delete(0, tk.END)
            self.y_entry.insert(0, str(y))
            log(f"Vald punkt: {self.selected_device} x={x}, y={y}")

    def move_point(self):
        if not self.selected_device:
            return
        try:
            x = float(self.x_entry.get())
            y = float(self.y_entry.get())
        except ValueError:
            return

        self.devices[self.selected_device]["pos"] = (x, y)
        log(f"{self.selected_device} flyttad till x={x}, y={y}")

    def clear_devices(self):
        to_delete = [k for k in self.devices.keys() if "Fake" not in str(k)]
        for key in to_delete:
            del self.devices[key]
        log("Alla anslutna enheter rensade.")
    
    def on_resize(self, event):
        self.redraw()

    def redraw(self):
        w, h = self.canvas.winfo_width(), self.canvas.winfo_height()
        if w < 10 or h < 10:
            return

        self.scale = min(w / self.image_width, h / self.image_height)

        new_w = int(self.image_width * self.scale)
        new_h = int(self.image_height * self.scale)

        self.scaled_image = self.original_image.resize((new_w, new_h), Image.LANCZOS)
        self.photo = ImageTk.PhotoImage(self.scaled_image)

        self.canvas.delete("all")
        self.canvas.create_image(0, 0, anchor="nw", image=self.photo)
        self.draw_rooms()  
        self.draw_all_points()

    def draw_all_points(self):
        self.canvas.delete("goal")
        for pid, tid in self.points.values():
            self.canvas.delete(pid)
            if tid is not None:
                if isinstance(tid, tuple):
                    # radera både rektangel och text
                    for t in tid:
                        self.canvas.delete(t)
                else:
                    self.canvas.delete(tid)

        self.points = {}
        self.canvas_object_map = {}

        for addr, info in list(self.devices.items()):
            x_esp, y_esp = info["pos"]
            goalx_esp, goaly_esp = info.get("goal", (None, None))
            battery = info.get("battery", 0)
            state = info.get("state", "null")
            alarm = info.get("alarm", False)

            sx, sy = self.esp_to_py(x_esp, y_esp)
            sx *= self.scale
            sy *= self.scale
            if "Label" in str(addr):
                if "ValObj" in str(addr):
                    pid = self.canvas.create_rectangle(
                        sx - 7, sy - 7, sx + 7, sy + 7,
                        fill="orange", outline="black"
                    )
                    text_id = self.canvas.create_text(
                        sx + 60, sy,
                        text="Valuable Object",
                        fill="black",
                        font=("Calibri", 11, "bold")
                    )
                    tid = (text_id,)

                elif "Charger" in str(addr):
                    pid = self.canvas.create_rectangle(
                        sx - 7, sy - 7, sx + 7, sy + 7,
                        fill="blue", outline="black"
                    )
                    text_id = self.canvas.create_text(
                        sx + 35, sy,
                        text="Charger",
                        fill="black",
                        font=("Calibri", 11, "bold")
                    )
                    tid = (text_id,)
                
                elif "Robot" in str(addr):
                    pid = self.canvas.create_oval(
                        sx - 7, sy - 7, sx + 7, sy + 7,
                        fill="red", outline="black"
                    )
                    text_id = self.canvas.create_text(
                        sx + 30, sy,
                        text="Robot",
                        fill="black",
                        font=("Calibri", 11, "bold")
                    )
                    tid = (text_id,)

            elif "ValObj" in str(addr):
                try:
                    vo_id = int(str(addr).replace("ValObj", ""))
                except ValueError:
                    vo_id = None

                is_stolen = vo_id in self.selected_vos if vo_id is not None else False
                fill_color = "grey" if is_stolen else "orange"

                pid = self.canvas.create_rectangle(
                    sx - 5, sy - 5, sx + 5, sy + 5,
                    fill=fill_color, outline="black"
                )
                tid = None

            elif "Charger" in str(addr):
                pid = self.canvas.create_rectangle(
                    sx - 5, sy - 5, sx + 5, sy + 5,
                    fill="blue", outline="black"
                )
                tid = None

            else:
                tid = None
                if goalx_esp is not None and goaly_esp is not None and goalx_esp != 0 and goaly_esp != 0:
                    gx, gy = self.esp_to_py(goalx_esp, goaly_esp)
                    gx *= self.scale
                    gy *= self.scale

                    g1 = self.canvas.create_line(
                        gx - 6, gy - 6, gx + 6, gy + 6,
                        fill="green", width=2,
                        tags=("goal",)
                    )

                    g2 = self.canvas.create_line(
                        gx - 6, gy + 6, gx + 6, gy - 6,
                        fill="green", width=2,
                        tags=("goal",)
                    )

                    # goal_text = self.canvas.create_text(
                    #     gx, gy - 14,
                    #     text=f"Robot {addr}'s goal",
                    #     fill="green",
                    #     font=("Calibri", 10, "bold"),
                    #     tags=("goal",)
                    # )

                    path = self.canvas.create_line(
                        sx, sy, gx, gy,
                        fill="green", dash=(4, 6),
                        tags=("goal",)
                    )
                    
                # Röd prick
                pid = self.canvas.create_oval(
                    sx - 5, sy - 5, sx + 5, sy + 5,
                    fill="red", outline="black"
                )
                if self.show_info_boxes:
                    #text_str = f"ID: {addr}\nState: {state}\nBattery: {battery}%\nAlarm: {alarm}"
                    text_lines = [
                        f"ID: {addr}",
                        f"State: {state}",
                        f"Battery: {battery}%"
                    ]

                    if alarm:
                        text_lines.append("Alarm: TRUE")

                    text_str = "\n".join(text_lines)


                    temp_id = self.canvas.create_text(0, 0, text=text_str, font=("Arial", 10, "bold"))
                    bbox = self.canvas.bbox(temp_id)  # (x1, y1, x2, y2)
                    self.canvas.delete(temp_id)

                    pad_x = 2
                    pad_y = 2
                    x1 = sx + 53 + bbox[0] - pad_x
                    y1 = sy - 33 + bbox[1] - pad_y
                    x2 = sx + 53 + bbox[2] + pad_x
                    y2 = sy - 33 + bbox[3] + pad_y

                    # 1) Bestäm grundfärg från state
                    fill_color = "whitesmoke"
                    if state == "Charging":
                        fill_color = "lightblue"
                    elif state == "Moving":
                        fill_color = "whitesmoke"
                    elif state == "Scanning":
                        fill_color = "orange"
                    elif state == "Planning":
                        fill_color = "lightgreen"
                    elif state == "Disconnected":
                        fill_color = "darkgray"
                    elif state == "Killed":
                        fill_color = "red"

                
                    if alarm:
                        fill_color = "red" if int(time.time()) % 2 == 0 else "white"

                    rect_id = self.canvas.create_rectangle(
                        x1, y1, x2, y2,
                        fill=fill_color,
                        outline="black"
                    )

                    text_color = "white" if alarm and fill_color == "red" else "black"

                    text_id = self.canvas.create_text(
                        sx + 53, sy - 33,
                        text=text_str,
                        fill=text_color,
                        font=("Calibri", 11, "bold")
                    )


                    tid = (rect_id, text_id)


            for p_id, p_data in self.person_list.items():
                # Get coordinates
                px_esp = p_data.get('x')
                py_esp = p_data.get('y')
            
                # Convert to Python coordinates
                sx, sy = self.esp_to_py(px_esp, py_esp)
                sx *= self.scale
                sy *= self.scale

                # Draw a Blue Oval to represent a Person
                p_icon = self.canvas.create_oval(
                    sx - 6, sy - 6, sx + 6, sy + 6,
                    fill="deep sky blue", outline="black", width=2
                )
            
                # Add a label so we know which person is which
                p_text = self.canvas.create_text(
                    sx, sy - 15,
                    text=f"Person {p_id}",
                    fill="blue",
                    font=("Calibri", 10, "bold")
                )

                # Store these IDs in self.points to make them clickable
                self.points[f"Person{p_id}"] = (p_icon, p_text)

            self.points[addr] = (pid, tid)
            self.canvas_object_map[pid] = addr


    def update_loop(self):
        self.draw_all_points()
        self.root.after(100, self.update_loop)

    def esp_to_py(self, x, y):
        x_ratio = (x - self.ESP_X_MIN) / (self.ESP_X_MAX - self.ESP_X_MIN)
        y_ratio = (y - self.ESP_Y_MIN) / (self.ESP_Y_MAX - self.ESP_Y_MIN)

        px = self.PY_X_MIN + x_ratio * (self.PY_X_MAX - self.PY_X_MIN)

        py = self.PY_Y_MAX - (y_ratio * (self.PY_Y_MAX - self.PY_Y_MIN))

        return px, py
    
    def update_room_info(self):
        text = """
    Room 1
    X: 4.5 -> 11
    Y: 47  -> 67.5

    Room 2
    X: 3.5 -> 11
    Y: 42  -> 47

    Room 3
    X: 4.5 -> 17
    Y: 19  -> 42

    Room 4
    X: 17  -> 42.5
    Y: 20.8 -> 29

    Room 5
    X: 20.3 -> 24
    Y: 23.5 -> 29

    Room 6
    X: 24.5 -> 28.3
    Y: 23.5 -> 29

    Room 7
    X: 28.7 -> 32.8
    Y: 23.5 -> 29

    Room 8
    X: 33.8 -> 37
    Y: 23.5 -> 29

    Room 9
    X: 37.3 -> 42.5
    Y: 23.5 -> 29

    Room 10
    X: 42.5 -> 45
    Y: 20.8 -> 29

    Room 11
    X: 1.5 -> 50.8
    Y: 12.5 -> 19

    Room 12
    X: 50.8 -> 62
    Y: 9.8 -> 19.8

    Room 13
    X: 45.5 -> 58.5
    Y: 19.8 -> 30.2

    Room 14
    X: 45.7 -> 54.5
    Y: 30.2 -> 42.5

    Room 15
    X: 44.5 -> 49.8
    Y: 0.5 -> 11.3

    Room 16
    X: 49.8 -> 58.5
    Y: 0.5 -> 9.8
    """

        self.room_text.configure(state="normal")
        self.room_text.delete("1.0", tk.END)
        self.room_text.insert(tk.END, text.strip())
        self.room_text.configure(state="disabled")

    def open_device_popup(self, device_id):
        self.devices[device_id].setdefault("obstacle_enabled", False)
        self.devices[device_id].setdefault("radar_enabled", False)

        win = tk.Toplevel(self.root)
        win.title(f"Device {device_id}")
        win.minsize(420, 200)

        win.columnconfigure(0, weight=1) 
        win.rowconfigure(2, weight=1)  

        # --- Header ---
        header = ttk.Frame(win, padding=(12, 12, 12, 6))
        header.grid(row=0, column=0, sticky="ew")
        header.columnconfigure(0, weight=1)

        ttk.Label(
            header,
            text=f"Device ID: {device_id}",
            font=("Calibri", 14, "bold")
        ).grid(row=0, column=0, sticky="w")

        ttk.Separator(win).grid(row=1, column=0, sticky="ew", padx=12)

        # --- Sektion 1: Batteri
        battery_section = ttk.LabelFrame(win, text="Battery", padding=12)
        battery_section.grid(row=2, column=0, sticky="ew", padx=12, pady=12)
        battery_section.columnconfigure(1, weight=1)

        ttk.Label(battery_section, text="Battery (%):").grid(row=0, column=0, sticky="w")

        batt_var = tk.StringVar()
        batt_entry = ttk.Entry(battery_section, textvariable=batt_var, width=12)
        batt_entry.grid(row=0, column=1, sticky="ew", padx=(10, 0))

        def on_battery_change(target="one"):
            try:
                new_batt = float(batt_var.get())
            except ValueError:
                log("Ogiltig batterinivå (ange t.ex. 42 eller 42.5).")
                return

            new_batt = max(0.0, min(100.0, new_batt))

            if target == "one":
                self.battery_commands[device_id] = new_batt

            elif target == "all":
                for dev_id in self.devices:
                    if isinstance(dev_id, int):
                        self.battery_commands[dev_id] = new_batt

        btn_row = ttk.Frame(battery_section)
        btn_row.grid(row=0, column=2, sticky="e", padx=(10, 0))

        ttk.Button(btn_row, text="Apply",
                command=lambda: on_battery_change("one")).grid(row=0, column=0)

        ttk.Button(btn_row, text="Apply to all",
                command=lambda: on_battery_change("all")).grid(row=0, column=1, padx=(8, 0))


        # --- Sektion 2: Kill och revive commands
        control_section = ttk.LabelFrame(win, text="Device control", padding=12)
        control_section.grid(row=3, column=0, sticky="ew", padx=12, pady=(0, 12))
        control_section.columnconfigure(0, weight=1)

        def on_kill_device():
            self.kill_commands.add(device_id)
            log(f"Kill request queued for device {device_id}")

        def on_revive_device():
            self.revive_commands.add(device_id)
            log(f"Revive request queued for device {device_id}")

        def on_kill_all():
            for dev_id in self.devices:
                if isinstance(dev_id, int):  # bara robotar
                    self.kill_commands.add(dev_id)
            log("Kill request queued for ALL devices")
        
        def on_revive_all():
            for dev_id in self.devices:
                if isinstance(dev_id, int):  # bara robotar
                    self.revive_commands.add(dev_id)
            log("Revive request queued for ALL devices")

        btns = ttk.Frame(control_section)
        btns.grid(row=0, column=0, sticky="w")

        ttk.Button(btns, text="Kill device", command=on_kill_device).grid(row=0, column=0, padx=(0, 8))
        ttk.Button(btns, text="Revive", command=on_revive_device).grid(row=0, column=1, padx=(0, 8))
        ttk.Button(btns, text="Kill all", command=on_kill_all).grid(row=0, column=2, padx=(0, 8))
        ttk.Button(btns, text="Revive all", command=on_revive_all).grid(row=0, column=3)

        # --- Sektion 3: Sensors
        sensors_section = ttk.LabelFrame(win, text="Activate sensors", padding=12)
        sensors_section.grid(row=5, column=0, sticky="ew", padx=12, pady=(0, 12))
        sensors_section.columnconfigure(0, weight=1)
        
        obstacle_var = tk.BooleanVar(
            value=self.devices[device_id]["obstacle_enabled"]
        )
        radar_var = tk.BooleanVar(
            value=self.devices[device_id]["radar_enabled"]
        )

        def apply_obstacle_one():
            enabled = obstacle_var.get()
            self.devices[device_id]["obstacle_enabled"] = enabled
            self.obstacle_commands[device_id] = enabled
            log(f"Obstacle detectors set to {enabled} for device {device_id}")

        def apply_radar_one():
            enabled = radar_var.get()
            self.devices[device_id]["radar_enabled"] = enabled
            self.radar_commands[device_id] = enabled
            log(f"Radar set to {enabled} for device {device_id}")

        obstacle_chk = ttk.Checkbutton(
            sensors_section,
            text="Obstacle detectors",
            variable=obstacle_var,
            command=apply_obstacle_one
        )
        obstacle_chk.grid(row=0, column=0, sticky="w")

        radar_chk = ttk.Checkbutton(
            sensors_section,
            text="Radar",
            variable=radar_var,
            command=apply_radar_one
        )
        radar_chk.grid(row=1, column=0, sticky="w", pady=(6, 0))

        # --- Sektion 4: Teleport
        teleport_section = ttk.LabelFrame(win, text="Teleport", padding=12)
        teleport_section.grid(row=4, column=0, sticky="ew", padx=12, pady=(0, 12))
        teleport_section.columnconfigure(1, weight=1)
        teleport_section.columnconfigure(3, weight=1)

        ttk.Label(teleport_section, text="X:").grid(row=0, column=0, sticky="w")
        x_var = tk.StringVar()
        x_entry = ttk.Entry(teleport_section, textvariable=x_var, width=10)
        x_entry.grid(row=0, column=1, sticky="ew", padx=(8, 12))

        ttk.Label(teleport_section, text="Y:").grid(row=0, column=2, sticky="w")
        y_var = tk.StringVar()
        y_entry = ttk.Entry(teleport_section, textvariable=y_var, width=10)
        y_entry.grid(row=0, column=3, sticky="ew", padx=(8, 12))

        ttk.Label(teleport_section, text="Room ID:").grid(row=1, column=0, sticky="w", pady=(8, 0))
        room_var = tk.StringVar()
        room_entry = ttk.Entry(teleport_section, textvariable=room_var, width=10)
        room_entry.grid(row=1, column=1, sticky="w", padx=(8, 12), pady=(8, 0))


        def on_apply_teleport(target="one"):
            try:
                x = float(x_var.get())
                y = float(y_var.get())
                room = int(room_var.get())
            except ValueError:
                log("Ogiltiga X/Y (ange t.ex. 12 eller 12.5).")
                return

            if target == "one":
                self.teleport_commands[device_id] = (x, y, room)
                log(f"Teleport queued for device {device_id}: x={x}, y={y}")

            elif target == "all":
                for dev_id in self.devices:
                    if isinstance(dev_id, int):  # bara robotar
                        self.teleport_commands[dev_id] = (x, y, room)
                log(f"Teleport queued for ALL devices: x={x}, y={y}, room={room}")

        btn_row = ttk.Frame(teleport_section)
        btn_row.grid(row=0, column=4, padx=(8, 0))
        ttk.Button(btn_row, text="Apply", command=lambda: on_apply_teleport("one")).grid(row=0, column=0)
        ttk.Button(btn_row, text="Apply to all", command=lambda: on_apply_teleport("all")).grid(row=0, column=1, padx=(8, 0))
        self.x_entry = x_entry
        self.y_entry = y_entry
        self.room_entry = room_entry


    def toggle_info_boxes(self):
        self.show_info_boxes = not self.show_info_boxes

        if self.show_info_boxes:
            self.toggle_info_btn.config(text="Hide info boxes")
        else:
            self.toggle_info_btn.config(text="Show info boxes")

        self.draw_all_points()

    def draw_rooms(self):
        if not self.show_rooms:
            return

        nbr = self.get_active_robot_count()
        room_groups = self.get_room_groups(nbr)

        # room_id -> group_id
        room_to_group = {}
        for g, rooms in room_groups.items():
            for r in rooms:
                room_to_group[r] = g

        GROUP_COLORS = {
            1: "#ff5555",
            2: "#55ff55",
            3: "#5555ff",
            4: "#ffaa00",
        }

        for room_id, x1, x2, y1, y2 in self.rooms:
            px1, py1 = self.esp_to_py(x1, y1)
            px2, py2 = self.esp_to_py(x2, y2)

            px1 *= self.scale
            py1 *= self.scale
            px2 *= self.scale
            py2 *= self.scale

            group_id = room_to_group.get(room_id)
            color = GROUP_COLORS.get(group_id, "gray")

            rect_id = self.canvas.create_rectangle(
                px1, py2, px2, py1,
                outline=color,
                width=4,
                tags=("room",)
            )

            self.canvas.tag_bind(
                rect_id, "<Button-1>",
                lambda e, x=x1, y=y1, room_id=room_id: self.on_room_click(x, y, room_id)
            )


    def get_active_robot_count(self):
        return sum(1 for dev_id in self.devices if isinstance(dev_id, int))
        

    
    def on_room_click(self, x, y, room_id):
        # Konvertera ESP → PY
        px, py = self.esp_to_py(x, y)

        log(f"Room {room_id} clicked. Setting teleport to x={px:.1f}, y={py:.1f}")

        # Kolla om teleport-popupen är öppen
        # Vi kollar bara om variabler finns (x_entry, y_entry, room_entry)
        try:
            self.x_entry.delete(0, tk.END)
            self.x_entry.insert(0, f"{px:.1f}")

            self.y_entry.delete(0, tk.END)
            self.y_entry.insert(0, f"{py:.1f}")

            self.room_entry.delete(0, tk.END)
            self.room_entry.insert(0, str(room_id))
        except AttributeError:
            # Om popup inte är öppen, skapa den automatiskt
            log("Teleport popup not open. Open it to auto-fill values.")

    def toggle_rooms(self):
        self.show_rooms = not self.show_rooms

        state = "normal" if self.show_rooms else "hidden"
        self.canvas.itemconfigure("room", state=state)

        if self.show_rooms:
            self.toggle_rooms_btn.config(text="Hide room boundaries")
        else:
            self.toggle_rooms_btn.config(text="Show room boundaries")

    def get_room_groups(self, nbr):
        groups = {}

        def add(g, r):
            groups.setdefault(g, []).append(r)

        if nbr == 1 or nbr is None:
            for r in [1,2,3,4,5,6,7,8,9,10,11,15,16,12,14,13]:
                add(1, r)

        elif nbr == 2:
            for r in [1,2,3,4,5,6,7,8,9,10]:
                add(1, r)
            for r in [11,15,16,12,14,13]:
                add(2, r)

        elif nbr == 3:
            for r in [1,2,3,4,5,6,7,8,9,10]:
                add(1, r)
            for r in [11,15,16]:
                add(2, r)
            for r in [12,14,13]:
                add(3, r)

        else:  # nbr >= 4
            for r in [1,2,3]:
                add(1, r)
            for r in [4,5,6,7,8,9,10]:
                add(2, r)
            for r in [11,15,16]:
                add(3, r)
            for r in [12,14,13]:
                add(4, r)

        return groups
    
    def redraw_rooms_only(self):
        """
        Ritar endast rumsgrupperna på nytt utan att påverka övriga enheter eller personer.
        """
        # Ta bort gamla rumsrektanglar
        self.canvas.delete("room")

        # Rita om rummen med nuvarande robot-count
        self.draw_rooms()
        self.canvas.update()
        self.root.update()









