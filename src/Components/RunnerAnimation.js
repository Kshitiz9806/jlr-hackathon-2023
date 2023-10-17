import React, {useState} from 'react'
import './RunnerAnimation.css'

function RunnerAnimation(props) {

  var l = props.left;
  l = l + "%";
  console.log();
  var car_style = {
    left: l,
  }

  return (
    <div className='runner-container-main'>
      <div className='white-sections' style={{top: "0%"}}>
      </div>
      <div className='white-sections' style={{top: "26%"}}>
      </div>
      <div className='white-sections' style={{top: "53%"}}>
      </div>
      <div className='white-sections'style={{top: "80%"}}>
      </div>
      <div className='car-on-road' style={car_style}>
      </div>
      <div className='sterring-prompt'>
        Steering: {props.steer_direction}
      </div>
    </div>
  )
}

export default RunnerAnimation