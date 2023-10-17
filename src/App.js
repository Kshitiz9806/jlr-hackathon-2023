import './App.css';
import Navbar from './Components/Navbar.js'
import ServerClientModel from './Components/ServerClientModel';
import SpamAttacks from './Components/SpamAttacks';
import FalseDataAttacks from './Components/FalseDataAttacks';
import EncryptedCommunication from './Components/EncryptedCommunication';
import { BrowserRouter, Routes, Route } from 'react-router-dom';

function App() {
  return (
    <div>
      <Routes>
        <Route path='/' element=<Navbar />/>
        <Route path='/server_client_model' element=<ServerClientModel />/>
        <Route path='/spam_attack' element=<SpamAttacks />/>
        <Route path='/false_data_attack' element=<FalseDataAttacks />/>
        <Route path='/secured_communication' element=<EncryptedCommunication />/>
      </Routes>
    </div>
  );
}

export default App;
