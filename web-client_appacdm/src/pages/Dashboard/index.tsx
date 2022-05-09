import React, { useMemo, useEffect, useState } from 'react';
import { Container, ChartContainer, DataContainer } from './styles';
import socketio from 'socket.io-client';

import ScatterChart from '../../components/ScatterChart';

interface IDataObject {
  name: string;
  data: number;
}

interface IDeviceDataObject {
  device: string;
  data: IDataObject[];
}

function Dashboard() {
  const [devDataMan, setdevDataMan] = useState<IDeviceDataObject>(
    {
      device: "", 
      data: 
      [
        { name: "", data: 0 },
        { name: "", data: 0 },
        { name: "", data: 0 }
      ]
    }
  );

  const [devDataPet, setdevDataPet] = useState<IDeviceDataObject>(
    {
      device: "", 
      data: 
      [
        { name: "", data: 0 },
        { name: "", data: 0 },
        { name: "", data: 0 }
      ]
    }
  );
  
  // Connect to the socket server
  const socket = useMemo(() => socketio('http://localhost:3030/', {
    transports : ['websocket'] 
  }), []);

  useEffect(() => {
    socket.on('devDataMan', (data) => {
      setdevDataMan(data);
    });

    socket.on('devDataPet', (data) => {
      setdevDataPet(data);
    });

    socket.on("disconnect", () => {
      console.log(`disconnect`);
    });
  }, []) 
  console.log(devDataMan);
  return (
    <ScatterChart EixoTransversal={devDataMan.data[0].data} EixoSagital={devDataMan.data[1].data } EixoLongitudinal={devDataMan.data[2].data } />    
  );
}

export default Dashboard;