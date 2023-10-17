import React, {useState} from 'react'
import './RunnerAnimation.css'
import carImage from './assets/car_image.png'

function RunnerAnimation(props) {

  var l = props.left;
  l = l + "%";
  var car_style = {
    left: l,
  }
  var renderElements;
  if(l==="-14%" || l==="-15%") {
    renderElements = <div className='alert-box'>Alert! Car has crashed</div>;
  }
  else {
    renderElements = <img src={carImage} className='car-on-road' style={car_style}></img>;
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
      {/* <div className='car-on-road' style={car_style}>
      </div> */}
      {/* <img src={carImage} className='car-on-road' style={car_style}></img> */}
      {renderElements}
      <div className='sterring-prompt'>
        Steering: {props.steer_direction}
      </div>
    </div>
  )
}

export default RunnerAnimation