import {ChildProcess, spawn} from 'child_process';

// TODO:
// 1. handle exit of modelling service process
// 2. maybe collect stderr output for logging.
// 3. handle exit/crash of modelling service process and restart it if needed (one day, the modelling service will back up its state)
// 4. handle errors

const modellingServicePath = '../../engines/build/e2_modellingService';

/* On initialisation, we spawn the modelling service as a child process. */
const modellingService = spawn(modellingServicePath);
console.log(`Spawned modelling service process ${modellingService.pid}`);

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
    const responsePromise = new Promise<string>((resolve, reject) => {
        modellingService.stdout.on('data', (data) => {
            resolve(data.toString());
        });
    });
    const responseText = await responsePromise;
    console.log(`Response from modelling service: ${responseText}`);                   //--- DEBUG ---

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



