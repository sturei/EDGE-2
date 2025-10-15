#!/bin/zsh

service_path="../build/e2_modellingService"

# Start the modelling service as a coprocess
coproc $service_path

echo "Modelling Service Interactive Client"
echo "Instructions:"
echo "  Enter each action like this: {\"type\":<string>, \"payload\":<any valid JSON>}."
echo "  The payload must be valid JSON e.g. a string, number, array or object."
echo "  Examples:"
echo "    {\"type\":\"addEmptyBody\", \"payload\":{}}"
echo "  Or, type 'q' to quit."

while true; do
    # Read a line of user input. Uses vared instead of read to allow editing of the input line
    user_input=""
    vared -p "Next action? " -c user_input
    [[ "$user_input" == "" ]] && continue

    # Check for quit command
    [[ "$user_input" == "q" ]] && break
    
    # Send the user input to the service
    print -p $user_input

    #sleep 1  # TODO: create a long-running action (e.g. sleep) so that I can test what happens.

    # Get the response from the service
    read -p service_response
    echo "$service_response"
done





