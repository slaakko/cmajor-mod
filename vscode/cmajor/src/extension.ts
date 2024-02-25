// The module 'vscode' contains the VS Code extensibility API
// Import the module and reference it with the alias vscode in your code below
import * as net from 'net';
import * as vscode from 'vscode';
import * as languageclient from 'vscode-languageclient/node';

let client : languageclient.LanguageClient;

let serverOptions : languageclient.ServerOptions 

 // This method is called when your extension is activated
// Your extension is activated the very first time the command is executed
export function activate(context: vscode.ExtensionContext) {

	// Use the console to output diagnostic information (console.log) and errors (console.error)
	// This line of code will only be executed once when your extension is activated
	console.log('Cmajor extension started');

	let connectionInfo = {
		port: 57000
	};

	serverOptions = () => {
		let socket = net.connect(connectionInfo);
		let result: languageclient.StreamInfo = {
			writer: socket,
			reader: socket
		};
		return Promise.resolve(result);
	};

	const clientOptions: languageclient.LanguageClientOptions = {
		documentSelector: [
			{ scheme: "file", language: "cmajor" }
		]
	}

	client = new languageclient.LanguageClient(
		'cmajor-language-server', "Cmajor Language Server", serverOptions, clientOptions);
	client.start();
}

// This method is called when your extension is deactivated
export function deactivate() {
	client.stop()
}
