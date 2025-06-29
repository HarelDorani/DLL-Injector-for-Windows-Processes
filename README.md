# DLL Injector for Windows Processes

This project demonstrates how to **inject a DLL** into another Windows process, specifically Visual Studio (`devenv.exe`), using the **Windows API**. The DLL, once injected, shows a simple message box to indicate success.

---

## 📌 Why DLL Injection?

DLL Injection is a powerful and low-level technique with both **legitimate** and **malicious** uses. Understanding it is essential for anyone diving deep into system programming, security research, or reverse engineering.

## 🔧 How it Works

1. Finds the process ID of `devenv.exe`.
2. Opens a handle to the process.
3. Allocates memory in the target process.
4. Writes the path to `injector.dll` into the allocated memory.
5. Creates a **remote thread** in the target process that calls `LoadLibraryA`, causing the DLL to be loaded.
6. Once loaded, the DLL runs `DllMain` and shows a message box.


### ✅ Legitimate Uses

- **Security Research / Penetration Testing**  
  Check if an application or system is vulnerable to unauthorized code execution.

- **Modding / Extending Applications**  
  Add features or behavior to apps/games without source code.

- **Debugging and Reverse Engineering**  
  Inject code to monitor or manipulate memory, hooks, or internal logic.

- **Custom Automation Tools**  
  Enable automation for apps that don't expose APIs, like adding keyboard shortcuts, capturing GUI data, etc.

- **Performance Monitoring / Hooking APIs**  
  Monitor internal system or application APIs for metrics or logging.


🔒 As a result, antivirus programs commonly flag DLL injection — **even in benign or educational tools**. Always use responsibly and legally.

> 💡 This project is for **educational and ethical purposes only**.
