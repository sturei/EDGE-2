import express from "express";
import cors from "cors"

import ViteExpress from "vite-express";
import { dispatchAction } from "./modelling/actions.js";

const app = express();
app.use(express.json());
app.use(cors());

app.get("/hello", (_, res) => {
  res.send("Hello Vite + React + TypeScript!");
});

/** Dispatches the specified action to the modelling service */
app.post("/modelling/actions", async (req, res) => {
  // TODO:
  // - error handling (return 400 for bad requests (e.g, malformed JSON), 500 for server-side errors, etc)
  // - tidy up the modelling engine to always return valid JSON on stdout, including error messages.
  // - add an action to get the current graphics state, and an app.get("modelling/actions") endpoint to get it via an action "{type: 'getGraphics', payload: {}}"
  // - However, note that the 'proper' (i.e much more efficient) way to get the graphics would be to stream the diffs through a websocket

  const action = req.body;
  const response = await dispatchAction(action);
  res.status(201).json({response:response});
});

ViteExpress.listen(app, 3000, () =>
  console.log("Server is listening on port 3000..."),
);
