import React, {useState, useEffect} from 'react'
import Navbar from './Navbar'
import Console from './Console'

import serverTextFile from './logs_data/log_server_dummy_1.log'
import clientTextFile from './logs_data/log_client_dummy_1.log'
import attackerTextFile from './logs_data/log_attacker_dummy_1.log'
import getLogs from './helper_functions/getLogs'

function SpamAttacks() {

  const [ serverLogs, setServerLogs ] = useState([]);
  const [ clientLogs, setClientLogs ] = useState([]);
  const [ attackerLogs, setAttackerLogs ] = useState([]);
  const [ count, setCount ] = useState(0);

  useEffect(() => {
    getLogs(serverTextFile)
    .then((response) => {
      setServerLogs(response[0]);
    })
    getLogs(clientTextFile)
    .then((response) => {
      setClientLogs(response[0]);
    })
    getLogs(attackerTextFile)
    .then((response) => {
      setAttackerLogs(response[0]);
    })
    const interval = setInterval(() => { 
      setCount(count + 1); 
    }, 2000)
  }, [count]);

  return (
    <div>
      <Navbar />
      <Console title={"Server Logs"} width={"50%"} height={"43.5vh"} height_inside={"38.5vh"} top={"13vh"} data={serverLogs}/>
      <Console title={"Client Logs"} width={"50%"} height={"43.5vh"} height_inside={"38.5vh"} top={"56.5vh"} data={clientLogs}/>
      <Console title={"Attacker Logs"} width={"50%"} height={"87vh"} height_inside={"82vh"} top={"13vh"} left={"50%"} data={attackerLogs}/>
    </div>
  )
}

export default SpamAttacks