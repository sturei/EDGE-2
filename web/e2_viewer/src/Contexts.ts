import { createContext } from 'react'
import { Document } from './document/document';        

export const DocumentContext =  createContext<Document>(new Document());