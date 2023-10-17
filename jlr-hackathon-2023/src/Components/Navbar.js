import React from 'react'
import './Navbar.css'
import { Link } from 'react-router-dom'

function Navbar() {
  return (
    <div className='navbar-main'>
      <div className='navbar-element'>
        <Link to="/server_client_model" className='link'>
          Server Client Operations
        </Link>
      </div>
      <div className='navbar-element'>
        <Link to='/spam_attack' className='link'>
          Spam Attacks Demonstration
        </Link>
      </div>
      <div className='navbar-element'>
        <Link to='/false_data_attack' className='link'>
          Hacker injects irregular data
        </Link>
      </div>
      <div className='navbar-element'>
        <Link to='/secured_communication' className='link'>
          Encrypted Communication
        </Link>
      </div>
    </div>
  )
}

export default Navbar