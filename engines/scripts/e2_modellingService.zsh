#!/bin/zsh

echo "Modelling Service Interactive Terminal Client"

read -r -d '' INSTRUCTIONS << EOM
Instructions:
  Enter each action like this: {"type":<string>, "payload":<any valid JSON>}.
  The payload must be valid JSON e.g. a string, number, array or object.
  Examples:
    {"type":"ping", "payload":{}}
    {"type":"addEmptyBody", "payload":{}}
    {"type":"addAcornBody", "payload":{"x":3, "y":2, "z":1}}
    {"type":"addWireRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}
    {"type":"addSheetRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}
    {"type":"addGPoint", "payload":{"size":0.2}}
    {"type":"addGLine", "payload":{"length":3}}
    {"type":"addGPlane", "payload":{"width":3, "height":2}}
    {"type":"addGSphere", "payload":{"radius":3}}
    {"type":"addGBlock", "payload":{"width":1, "height":2, "depth":3}}
  Type 'q' to quit, or 'h' to show these instructions again.
EOM

echo "$INSTRUCTIONS"

# Start the modelling service as a coprocess
coproc ../build/e2_modellingService

while true; do
    user_input=""
    service_response=""
    # Read a line of user input. Uses vared instead of read to allow editing of the input line
    vared -p "Next action? " -c user_input

    # Skip empty lines
    [[ -z "$user_input" ]] && continue      

    # Check for quit command
    [[ "$user_input" == "q" ]] && break

    # Check for help command
    [[ "$user_input" == "h" ]] && { echo "$INSTRUCTIONS"; continue; }

    # Send the user input to the service
    print -p $user_input

    # Get the response from the service
    read -p service_response
    echo "$service_response"
done





