#!/bin/zsh

echo "Modelling Service Interactive Terminal Client"

read -r -d '' INSTRUCTIONS << EOM
Instructions:
  Enter each action like this: {"type":<string>, "payload":<any valid JSON>}.
  The payload must be valid JSON e.g. a string, number, array or object.
  Examples:
    {"type":"Modeller::ping", "payload":{}}
    {"type":"Layout::addPrimitive", "payload":{"primitiveType":"rectangle", "width":3, "height":2, "pathName":"shape/profiles/rectangle1"}}
    {"type":"Layout::addPrimitive", "payload":{"primitiveType":"circle", "radius":1.5, "pathName":"shape/profiles/circle1"}}
    {"type":"Layout::addPrimitive", "payload":{"primitiveType":"roundRect", "width":3, "height":2, "cornerRadius":0.2, "pathName":"shape/profiles/roundRect1"}}
    {"type":"Features::addPrimitive", "payload":{"primitiveType":"block", "width":3, "height":2, "depth":1, "pathName":"shape/features/block1"}}
    {"type":"Features::addPrimitive", "payload":{"primitiveType":"sphere", "radius":1.5, "pathName":"shape/features/sphere1"}}
    {"type":"Features::addPrimitive", "payload":{"primitiveType":"cylinder", "radius":1.5, "depth":3, "pathName":"shape/features/cylinder1"}}
    {"type":"Features::addExtrusion", "payload":{"depth":3, "profilePathName":"shape/profiles/roundRect1", "pathName":"shape/features/extrusion1"}}

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





