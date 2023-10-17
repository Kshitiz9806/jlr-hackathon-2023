import React, { useEffect } from 'react'
import './Console.css'


function Console(props) {

  var inline_style = {
    width: props.width,
    height: props.height,
    left: props.left,
    top: props.top,
    overflowY: 'scroll',
  }

  var ConsoleEnd;

  useEffect(() => {
    ConsoleEnd.scrollIntoView({ behavior: "smooth" })
  })

  var render_content;

  render_content = props.data;
  console.log(props)

  return (
    <div className='console-main' style={inline_style}>
      <div className='console-head'>
        {props.title}
      </div>
      <div className='console-data'> 
        <ul>
          {render_content}
        </ul>
      </div>
      <div  ref={(el) => { ConsoleEnd = el; }}></div>
    </div>
  )
}

export default Console