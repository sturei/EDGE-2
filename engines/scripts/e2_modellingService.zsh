#!/bin/zsh

echo "Modelling Service Interactive Terminal Client"

read -r -d '' INSTRUCTIONS << EOM
Instructions:
  Enter each action like this: {"type":<string>, "payload":<any valid JSON>}.
  The payload must be valid JSON e.g. a string, number, array or object.
  Examples:
    {"type":"Modeller::ping", "payload":{}}
    {"type":"Sketches::addEmptyBody", "payload":{}}
    {"type":"Sketches::addAcornBody", "payload":{"x":3, "y":2, "z":1}}
    {"type":"Sketches::addRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}
    {"type":"Sketches::addRoundRect", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "cornerRadius":0.2}}
    {"type":"Profiles::addRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}
    {"type":"Profiles::addRoundRect", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "cornerRadius":0.2}}
    {"type":"Objects::addEmptyObject", "payload":{}}
    {"type":"Objects::addSphereObject", "payload":{"radius":1.5}}
    {"type":"Objects::addBlockObject", "payload":{"width":3, "height":2, "depth":1}}
    {"type":"Objects::addCylinderObject", "payload":{"radius":1.5, "depth":3}}
    {"type":"Objects::addInfiniteRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}}}
    {"type":"Objects::addCappedRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "depth":1}}
    {"type":"Objects::addExtrudedRectangle", "payload":{"lowerLeft":{"x":0, "y":0, "z":0}, "upperRight":{"x":3, "y":2, "z":0}, "depth":1}}
    {"type":"Features::addPrimitive", "payload":{"primitiveType" : "block", "width":3, "height":2, "depth":1, "pathName":"shape/features/block1"}}
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





