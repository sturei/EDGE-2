import {ChildProcess, spawn} from 'child_process';
import { Readable } from 'stream';

// TODO:
// 1. handle exit of modelling service process
// 2. maybe collect stderr output for logging.
// 3. handle exit/crash of modelling service process and restart it if needed (one day, the modelling service will back up its state)
// 4. handle errors

const modellingServicePath = '../../engines/build/e2_modellingService';

/* On initialisation, we spawn the modelling service as a child process. */
const modellingService = spawn(modellingServicePath);
console.log(`Spawned modelling service process ${modellingService.pid}`);


function readStream(stream: Readable, encoding: BufferEncoding = "utf8") : Promise<string> {
    stream.setEncoding(encoding);
    return new Promise((resolve, reject) => {
        let data:string = "";
        stream.on("data", (chunk) => {
            //console.log(`Received chunk: ${chunk.length} bytes`);   //--- DEBUG ---
            data += chunk;
            if (data.endsWith("\n")) {
                //console.log(`Received line: ${data.length} bytes`);   //--- DEBUG ---
                data = data.slice(0, -1); // remove trailing newline
                resolve(data);
                data="";
            }
        });
        stream.on("end", () => {
            console.log(`Stream ended: ${data.length} bytes`);   //--- DEBUG ---
            resolve(data);
            data="";
        });
        stream.on("error", (error) => {
            console.log(`Stream error: ${error} `);   //--- DEBUG ---
            reject(error);
        });
    });
}

modellingService.stderr.setEncoding('utf8');
modellingService.stderr.on('data', function(data) {
    console.log('stderr: ' + data);
});
modellingService.on('close', function(code) {
    console.log('Modelling service closed with code: ' + code);
});

/** Dispatches the specified action to the modelling service and waits for a response. 
 * The action argument and the return value are JSON objects */
export async function dispatchAction(action: any) : Promise<any> {

    // Send the action to the modelling service as a JSON string.
    const actionText = JSON.stringify(action);
    console.log(`Dispatching action to modelling service: `, actionText);      //--- DEBUG ---
    modellingService.stdin.write(actionText + '\n');

    // Wait for the response from the modelling service
    const responseText = await readStream(modellingService.stdout); 
    console.log(`Response from modelling service: ${responseText.length} bytes`);                   //--- DEBUG ---

    // process the response. It consist of a JSON string, which we unpack into a JSON object.
    let response = JSON.parse(responseText);

    // client actions are JSON strings embedded into the response object. 
    // We unpack them too, so that whole returned value is a regular JSON object.
    let clientActions = [];
    for (const actionText of response.clientActions) {
        clientActions.push(JSON.parse(actionText));
    }
    response.clientActions = clientActions;

    return response;
}

export { modellingService };     // export the spawned process for testing purposes



