function getLastSteerDirection(log) {
  var words = log.split(' ');
  var n = words.length;
  return [words[n-2], words[n-1]]
}

async function getLogs(file) {
  var file_reader = await fetch(file);
  var text = await file_reader.text();
  let logs = text.split('\n');
  var logsRender = logs.map((log) => <><li className='single-log'>{log}</li> <br/></>);
  const [ left, steer ] = getLastSteerDirection(logs[logs.length-1]);
  return [logsRender, left, steer];
}

export default getLogs;