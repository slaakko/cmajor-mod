"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.deactivate = exports.activate = void 0;
// The module 'vscode' contains the VS Code extensibility API
// Import the module and reference it with the alias vscode in your code below
const net = require("net");
const languageclient = require("vscode-languageclient/node");
let client;
let serverOptions;
// This method is called when your extension is activated
// Your extension is activated the very first time the command is executed
function activate(context) {
    // Use the console to output diagnostic information (console.log) and errors (console.error)
    // This line of code will only be executed once when your extension is activated
    console.log('Cmajor extension started');
    let connectionInfo = {
        port: 57000
    };
    serverOptions = () => {
        let socket = net.connect(connectionInfo);
        let result = {
            writer: socket,
            reader: socket
        };
        return Promise.resolve(result);
    };
    const clientOptions = {
        documentSelector: [
            { scheme: "file", language: "cmajor" }
        ]
    };
    client = new languageclient.LanguageClient('cmajor-language-server', "Cmajor Language Server", serverOptions, clientOptions);
    client.start();
}
exports.activate = activate;
// This method is called when your extension is deactivated
function deactivate() {
    client.stop();
}
exports.deactivate = deactivate;
//# sourceMappingURL=extension.js.map