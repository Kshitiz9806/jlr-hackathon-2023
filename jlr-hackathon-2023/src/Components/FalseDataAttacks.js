import React, {useState, useEffect} from 'react'
import Navbar from './Navbar'
import Console from './Console'

import serverTextFile from './logs_data/log_server_dummy_1.log'
import clientTextFile from './logs_data/log_client_dummy_1.log'
import attackerTextFile from './logs_data/log_attacker_dummy_1.log'
import getLogs from './helper_functions/getLogs'
import RunnerAnimation from './RunnerAnimation'

function FalseDataAttacks() {

  const [ serverLogs, setServerLogs ] = useState([]);
  const [ clientLogs, setClientLogs ] = useState([]);
  const [ attackerLogs, setAttackerLogs ] = useState([]);
  const [ left, setLeft ] = useState("41");
  const [ steer, setSteer ] = useState("none");
  const [ count, setCount ] = useState(0);

  useEffect(() => {
    getLogs(serverTextFile)
    .then((response) => {
      setServerLogs(response[0]);
    })
    getLogs(clientTextFile)
    .then((response) => {
      setLeft(response[1]);
      setSteer(response[2]);
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
      <Console title={"Server Logs"} width={"50%"} height={"29vh"} height_inside={"24vh"} top={"13vh"} data={serverLogs}/>
      <Console title={"Client Logs"} width={"50%"} height={"29vh"} height_inside={"24vh"} top={"42vh"} data={clientLogs}/>
      <Console title={"Attacker Logs"} width={"50%"} height={"29vh"} height_inside={"24vh"} top={"71vh"} data={attackerLogs}/>
      <RunnerAnimation left={left} steer_direction={steer}/>
    </div>
  )
}

export default FalseDataAttacks