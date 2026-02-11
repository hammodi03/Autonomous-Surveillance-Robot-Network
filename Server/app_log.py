import logging
import queue
import tkinter as tk
from tkinter import ttk
from tkinter.scrolledtext import ScrolledText
from datetime import datetime
import re
from dataclasses import dataclass
from typing import Optional, Set


@dataclass(frozen=True)
class LogEvent:
    line: str
    agent_id: Optional[int] = None


_log_queue: "queue.Queue[LogEvent]" = queue.Queue()

_logger = logging.getLogger("app")
_logger.setLevel(logging.INFO)
_logger.propagate = False

if not _logger.handlers:
    fh = logging.FileHandler("app.log", encoding="utf-8")
    fh.setFormatter(logging.Formatter("%(asctime)s - %(levelname)s - %(message)s"))
    _logger.addHandler(fh)

_AGENT_RE = re.compile(r"\bAgent id:\s*(\d+)\b", re.IGNORECASE)


class LogWindow:
    def __init__(self, root: tk.Tk, title: str = "Logg"):
        self.root = root
        self.win = tk.Toplevel(root)
        self.win.title(title)
        self.win.geometry("900x420")

        self._agent_ids_seen: Set[int] = set()
        self._filter_agent_id: Optional[int] = None  # None = All

        top = ttk.Frame(self.win, padding=(8, 8, 8, 4))
        top.pack(fill="x")

        ttk.Label(top, text="Filter Agent ID:").pack(side="left")

        self.filter_var = tk.StringVar(value="All")
        self.filter_combo = ttk.Combobox(
            top,
            textvariable=self.filter_var,
            values=["All"],
            width=12,
            state="readonly",
        )
        self.filter_combo.pack(side="left", padx=(8, 8))
        self.filter_combo.bind("<<ComboboxSelected>>", self._on_filter_change)

        ttk.Button(top, text="Clear", command=self._clear_log_view).pack(side="left")

        self.text = ScrolledText(self.win, wrap="word")
        self.text.pack(fill="both", expand=True, padx=8, pady=(4, 8))
        self.text.configure(state="disabled")

        self._poll()

    def _on_filter_change(self, _evt=None) -> None:
        val = self.filter_var.get()
        if val == "All":
            self._filter_agent_id = None
        else:
            try:
                self._filter_agent_id = int(val)
            except ValueError:
                self._filter_agent_id = None

        self._append(f"--- Filter set to: {val} ---")

    def _append(self, line: str) -> None:
        self.text.configure(state="normal")
        self.text.insert("end", line + "\n")
        self.text.see("end")
        self.text.configure(state="disabled")

    def _update_filter_values(self) -> None:
        values = ["All"] + [str(x) for x in sorted(self._agent_ids_seen)]
        self.filter_combo["values"] = values
        if self.filter_var.get() not in values:
            self.filter_var.set("All")
            self._filter_agent_id = None

    def _poll(self) -> None:
        changed = False

        while True:
            try:
                ev = _log_queue.get_nowait()
            except queue.Empty:
                break

            if ev.agent_id is not None and ev.agent_id not in self._agent_ids_seen:
                self._agent_ids_seen.add(ev.agent_id)
                changed = True

            if self._filter_agent_id is None or ev.agent_id == self._filter_agent_id:
                self._append(ev.line)

        if changed:
            self._update_filter_values()

        self.root.after(100, self._poll)

    def _clear_log_view(self) -> None:
        self.text.configure(state="normal")
        self.text.delete("1.0", "end")
        self.text.configure(state="disabled")
        self._append("--- Log view cleared ---")


def init_log_window(root: tk.Tk) -> LogWindow:
    lw = LogWindow(root)
    log("Loggfönster initierat")
    return lw


def log(message: str, **fields) -> None:
    extra = ""
    if fields:
        extra = " | " + ", ".join(f"{k}={v}" for k, v in fields.items())

    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    line = f"{timestamp} - {message}{extra}"

    agent_id: Optional[int] = None
    if "agent_id" in fields:
        try:
            agent_id = int(fields["agent_id"])
        except Exception:
            agent_id = None
    else:
        m = _AGENT_RE.search(message)
        if m:
            agent_id = int(m.group(1))

    _log_queue.put(LogEvent(line=line, agent_id=agent_id))
    _logger.info(f"{message}{extra}")
