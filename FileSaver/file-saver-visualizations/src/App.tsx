import React, { useEffect, useState } from "react";
import "./App.css";
import ReactEcharts from "echarts-for-react";

const App: React.FC = () => {
  const [filesPerSecondIndex, setFilesPerSecondIndex] = useState(0);
  const [filesPerSecond, setFilesPerSecond] = useState([]);

  const pushFilesPerSecond = (n: number) => {
    const now = new Date();
    if (filesPerSecond.length > 100) {
      filesPerSecond.shift();
    }

    // @ts-ignore
    filesPerSecond.push([now.getTime(), n]);

    setFilesPerSecondIndex(filesPerSecondIndex + 1);
    setFilesPerSecond(filesPerSecond);
  };

  const buildOption = (filesPerSecond: [number, number][]) => ({
    xAxis: {
      type: "time",
      splitLine: {
        show: false
      }
    },
    yAxis: {
      boundaryGap: [0, "100%"],
      type: "value"
    },
    series: [
      {
        type: "line",
        data: filesPerSecond.concat([])
      }
    ]
  });

  useEffect(() => {
    const listener = (filesPerSecond: number) => {
      console.log("Pushing files per second");
      pushFilesPerSecond(filesPerSecond);
    };

    // @ts-ignore
    window.filesaver.on("filesPerSecond", listener);
    // @ts-ignore
    return () => window.filesaver.off("filesPerSecond", listener);
  });

  return (
    <div className="App">
      <ReactEcharts
        style={{ height: 300, width: "100%" }}
        option={buildOption(filesPerSecond)}
        notMerge
      />
    </div>
  );
};

export default App;
