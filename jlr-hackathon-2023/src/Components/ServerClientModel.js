import React, { useState, useEffect } from 'react'
import Navbar from './Navbar'
import Console from './Console'
import RunnerAnimation from './RunnerAnimation'

import serverTextFile from './logs_data/log_server_dummy_1.log'
import clientTextFile from './logs_data/log_client_dummy_1.log'
import getLogs from './helper_functions/getLogs'

function ServerClientModel() {

  const [ serverLogs, setServerLogs ] = useState([]);
  const [ clientLogs, setClientLogs ] = useState([]);
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
    const interval = setInterval(() => { 
      setCount(count + 1); 
    }, 2000)
  }, [count]);

  return (
    <div>
      <Navbar />
      <Console title={"Server Logs"} width={"50%"} height={"43.5vh"} top={"13vh"} data={serverLogs}/>
      <Console title={"Client Logs"} width={"50%"} height={"43.5vh"} top={"56.5vh"} data={clientLogs}/>
      <RunnerAnimation left={left} steer_direction={steer}/>
    </div>
  )
}

export default ServerClientModel