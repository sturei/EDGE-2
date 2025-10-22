#!/bin/zsh

echo "Desktop App Command-line Client"
echo "Instructions:"
echo "  Enter each action like this: {\"type\":<string>, \"payload\":<any valid JSON>}."
echo "  The payload must be valid JSON e.g. a string, number, array or object."
echo "  Examples:"
echo "    {\"type\":\"ping\", \"payload\":{}}"
echo "    {\"type\":\"addGBlock\", \"payload\":{}}"
echo "    {\"type\":\"addGBlock\", \"payload\":{\"length\":1, \"width\":2, \"height\":3}}"
echo "  Or, type 'q' to quit."

# Start the desktop client as a coprocess
coproc ../build/e2_desktopApp --magnum-log quiet

while true; do
    # Read a line of user input. Uses vared instead of read to allow editing of the input line
    user_input=""
    vared -p "Next action? " -c user_input

    # Ignore empty lines
    [[ "$user_input" == "" ]] && continue

    # Check for quit command
    [[ "$user_input" == "q" ]] && break
    
    # Send the user input to the service
    print -p $user_input

    # Get the response from the service. TODO: check that it's an ACK
    read -p service_response
    echo "$service_response"
done





