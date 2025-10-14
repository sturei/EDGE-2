#!/bin/zsh

service_path="../build/e2_modellingService"

# Start the modelling service as a coprocess
coproc $service_path

echo "Modelling Service Interactive Client"
echo "Instructions:"
echo "  Enter each action like this: {\"type\":\"<string>\", \"payload\":<any valid JSON>}."
echo "  The payload is optional. If supplied, it must be valid JSON e.g. a string, number, array or object."
echo "  Or, type 'q' to quit."

while true; do
    # Read a line of user input. Uses vared instead of read to allow editing of the input line
    user_input=""
    vared -p "Next action? " -c user_input

    # Check for quit command
    [[ "$user_input" == "q" ]] && break
    
    # Send the user input to the service
    print -p $user_input

    # Get the response from the service
    response=""
    while true; do
        read -p line
        [[ "$line" == "" ]] && break
        response+="$line\n"
    done
    echo $response

done





