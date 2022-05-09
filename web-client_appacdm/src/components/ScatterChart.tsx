import React from 'react';
import { Scatter } from 'react-chartjs-2';
import logo from '../assets/img/pointer.png'; // with import
import {Container} from './styles';

interface ScatterChartProps {
  EixoSagital: number        // X
  EixoTransversal : number   // Y
  EixoLongitudinal : number;   // Z
}


const ScatterChart = ({EixoTransversal, EixoSagital, EixoLongitudinal}: ScatterChartProps) => {

  const img = new Image();
  img.src = logo;

  const data = {
    datasets: [
      {
     
        label: 'Paciente',
        backgroundColor: 'rgba(255, 255, 255, 1)',
        borderColor: 'rgba(00, 00, 00, 1)',
  
        data: [
          { x: EixoSagital, y: EixoTransversal }
        ],
  
        pointStyle: [img, 'rect', 'triangle', 'circle'],
        pointRadius: 25,
        pointHoverRadius: 25,
        rotation: EixoLongitudinal,
      },
    ],
  };

  const options = {
    responsive: true,
    plugins: {
      title: {
        display: false,
        text: 'Chart.js Scatter Multi Axis Chart'
      },
    },
    scales: {
      xAxes: {
        display: true,
        position: 'center' as const,
        min: -90,
        max: 90,

        ticks: {
          stepSize: 10
        }

      },
      yAxes: {
        position: 'center' as const,
        min: -90,
        max: 90,
     
        display: true,
        ticks: {
          stepSize: 10
        },

      }
    }
  };

  return(
    <>
      <Scatter data={data} options={options}/>
    </>
  );
}

export default ScatterChart;

/*

// X

        title: {
          display: true,
          text: 'Eixo Sagital',
          color: '#000',
          align: 'center',
          font: {
            family: 'Arial',
            size: 10,
            weight: 'normal',
            lineHeight: 0.2,
          },
          padding: {top: 0, left: 0, right: 0, bottom: 0}
        },

        ticks: {
          callback: function(value: number) {
            return value + '°';
          },
          stepSize: 10
        }


// Y
        display: true,
        title: {
          display: true,
          text: 'Eixo Transversal',
          align: 'start' as const,
          color: '#000',
       
          font: {
            family: 'Arial',
            size: 10,
            weight: 'normal',
            lineHeight: 1.2,
          },
          padding: {top: 0, left: 0, right: 0, bottom: 0}
        },
        ticks: {
          callback: function(value: number) {
            return value + '°';
          },
          stepSize: 10
        },

*/