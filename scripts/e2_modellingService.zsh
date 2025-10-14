#!/bin/zsh

service_path="../build/e2_modellingService"

# Start the service and attach file descriptor 3 to its stdin
# TODO: figure out how to also capture its stdout and stderr

exec 3> >( "$service_path" )

echo "Modelling Service Interactive Client"
echo "Instructions:"
echo "  Enter each action like this: {\"type\":\"<a string defining the type of the action>\", \"payload\":<any valid JSON>}."
echo "  The payload is optional. If supplied, it must be valid JSON e.g. a string, number, array or object."
echo "  Or, type 'q' to quit."

while true; do
    # Read a line of user input. Uses vared instead of read to allow editing of the input line
    user_input=""
    vared -p "Next action? " -c user_input

    # Check for quit command
    [[ "$user_input" == "q" ]] && break
    
    # Send the user input to the service
    echo "$user_input" >&3 

done

# Close the file descriptor to signal EOF to the service
exec 3>&-




