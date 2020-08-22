[@react.component]
let make = () => {
  let (gTime, setGTime) = React.useState(() => Time.newDuration());
  let (isTimerActive, setIsTimerActive) = React.useState(() => false);
  let (startingPointInTime, setStartingPointInTime) =
    React.useState(() => Js.Date.now());

  React.useEffect1(
    () => {
      let intervalId: Pervasives.ref(option(Js.Global.intervalId)) =
        ref(None);
      if (isTimerActive) {
        intervalId :=
          Some(
            Js.Global.setInterval(
              () => {
                setGTime(_ =>
                  Time.normalize({
                    minutes: 0,
                    seconds:
                      int_of_float(
                        (Js.Date.now() -. startingPointInTime) /. 1000.0,
                      ),
                  })
                )
              },
              1000,
            ),
          );
      } else if (!isTimerActive) {
        let _ =
          switch (intervalId^) {
          | Some(a) => Some(() => {Js.Global.clearInterval(a)})
          | None => None
          };
        ();
      };

      switch (intervalId^) {
      | Some(a) => Some(() => Js.Global.clearInterval(a))
      | None => None
      };
    },
    [|isTimerActive|],
  );

  let onPlay = (): unit => {
    if (gTime == Time.newDuration()) {
      setStartingPointInTime(_ => Js.Date.now());
    } else {
      setStartingPointInTime(_ =>
        Js.Date.now() -. float_of_int(Time.toMilliseconds(gTime))
      );
    };
    setIsTimerActive(_ => true);
  };
  let onPause = (): unit => setIsTimerActive(_ => false);
  let onReset = (): unit => {
    setGTime(_ => Time.newDuration());
    setIsTimerActive(_ => false);
  };

  <div
    style={ReactDOM.Style.make(
      ~width="100%",
      ~height="100%",
      ~display="flex",
      ~flexDirection="column",
      ~alignItems="center",
      ~alignContent="center",
      (),
    )}>
    <div style={ReactDOM.Style.make(~height="2%", ())} />
    <TimerView
      limit={minutes: 45, seconds: 0}
      time=gTime
      onPlay
      onPause
      onReset
    />
    <div style={ReactDOM.Style.make(~height="2%", ())} />
    <TextArea time=gTime />
  </div>;
};
