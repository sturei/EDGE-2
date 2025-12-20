import express from "express"
import cors from "cors"
import { promises as fs } from 'fs'

import ViteExpress from "vite-express";
import { dispatchAction } from "./modelling/actions.js";

const app = express();
app.use(express.json());
app.use(cors());

const routes = {
    modellingActions: "/modelling/actions",
    macros: "/macros",
    macro: "/macros/:filePath",   
}

const macrosPath = 'public/macros';

// sanity check
app.get("/hello", (_, res) => {
    res.send("Hello Vite + React + TypeScript!");
});

/** Dispatches the specified action to the modelling service */
app.post(routes.modellingActions, async (req, res) => {

    // TODO:error handling (e.g. return 400 for invalid input, 500 for internal errors, etc)

    // send the request
    const action = req.body;

    // wait for the response
    const response = await dispatchAction(action);

    // and return it
    res.status(201).json({response:response});
});

/** Lists the available macros */
app.get(routes.macros, async (req, res) => {
    console.log('GET:' + routes.macros);
    try {
        const files = await fs.readdir(macrosPath);
        res.json(files);
    }
    catch (e) {
        res.status(500).json(e);
    }
})

/** Retrieves the specified macro */
app.get(routes.macro, async (req, res) => {
    const { filePath } = req.params;
    console.log('GET:' + filePath);
    const fullPath = `${macrosPath}/${filePath}`;
    try {
        const fileContent = await fs.readFile(fullPath, 'utf-8');
        res.json({ content: fileContent });
    } catch (e) {
        res.status(500).json(e);
    }
});

ViteExpress.listen(app, 3000, () =>
    console.log("Server is listening on port 3000..."),
);
