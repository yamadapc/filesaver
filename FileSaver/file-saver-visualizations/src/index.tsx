import React from "react";
import ReactDOM from "react-dom";
import EventEmitter from "events";

import "./index.css";
import App from "./App";
import * as serviceWorker from "./serviceWorker";

// @ts-ignore
window.filesaver = new EventEmitter();

ReactDOM.render(<App />, document.getElementById("root"));

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: https://bit.ly/CRA-PWA
serviceWorker.unregister();
