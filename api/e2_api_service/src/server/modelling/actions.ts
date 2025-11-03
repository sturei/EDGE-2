import {ChildProcess, spawn} from 'child_process';

// TODO:
// 1. handle exit of modelling service process
// 2. maybe collect stderr output for logging.
// 3. handle exit/crash of modelling service process and restart it if needed (one day, the modelling service will back up its state)
// 4. handle errors

const modellingServicePath = '../../engines/build/e2_modellingService';
let modellingService = spawn(modellingServicePath);

console.log(`Spawned modelling service process ${modellingService.pid}`);

// TODO make this take a string and return a string, for consistency with itself.
// also make it process the client actions instead of main doing it.
/** dispatches the specified action to the modelling service and waits for a response */
export async function dispatchAction(action: { type: string; payload: any }) : Promise<string> {

    console.log(`Dispatching action to modelling service: `, JSON.stringify(action));

    // Send an action to the modelling service
    modellingService.stdin.write(JSON.stringify(action) + '\n');

    // Wait for the response from the modelling service
    const response = new Promise<string>((resolve, reject) => {
        modellingService.stdout.on('data', (data) => {
            resolve(data.toString());
        });
    });
    const data = await response;

    // Return it
    console.log(`Response from modelling service: ${data}`);

    return data;
}

export { modellingService };



