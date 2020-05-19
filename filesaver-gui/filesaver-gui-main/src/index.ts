import { app, BrowserWindow } from "electron";
import { getObservable } from "./stats-observable";

if (require("electron-squirrel-startup")) {
  app.quit();
}

function createWindow() {
  const mainWindow = new BrowserWindow({
    height: 600,
    width: 800,
  });

  mainWindow.loadURL("http://localhost:3000");

  const statsSubscription = getObservable().subscribe((stats) => {
    mainWindow.webContents.send("stats", stats);
  });

  app.on("quit", () => {
    statsSubscription.unsubscribe();
  });
}

app.on("ready", createWindow);

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

app.on("activate", () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});
