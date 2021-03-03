import React from 'react';
import ReactDOM from 'react-dom';
import './bootstrap.css';
import { App } from './App';
import { HashRouter } from "react-router-dom";

ReactDOM.render(
  <React.StrictMode>
    <HashRouter hashType="noslash">
      <App />
    </HashRouter>
  </React.StrictMode>,
  document.getElementById('root')
);
