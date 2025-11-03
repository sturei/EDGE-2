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

    // TODO:error handling (e.g. return 400 for invalid input, 500 for internal errors, etc)

    // send the request
    const action = req.body;

    // wait for the response
    const response = await dispatchAction(action);

    // and return it
    res.status(201).json({response:response});
});

ViteExpress.listen(app, 3000, () =>
    console.log("Server is listening on port 3000..."),
);
