type onBeginClosing = Animation.onStop;
type action =
  | Open(Animation.onStop)
  | BeginClosing(onBeginClosing, Animation.onStop)
  | Close(Animation.onStop)
  | Animate(float, Animation.onStop)
  | Height(float);
type state = {
  height: float,
  animation: SpringAnimation.t,
};

let component = ReasonReact.reducerComponent("AnimateHeight");
let make = (~rAction, ~targetHeight, children) => {
  ...component,
  initialState: () => {
    height: targetHeight,
    animation: SpringAnimation.create(0.),
  },
  didMount: ({send}) => {
    rAction |> RemoteAction.subscribe(~send) |> ignore;
    Animate(targetHeight, None) |> send;
  },
  reducer: (action, state) =>
    switch (action) {
    | Height(v) => Update({...state, height: v})
    | Animate(finalValue, onStop) =>
      SideEffects(
        (
          ({send}) =>
            state.animation
            |> SpringAnimation.setOnChange(
                 ~finalValue, ~precision=10., ~onStop, ~onChange=h =>
                 Height(h) |> send
               )
        ),
      )
    | Close(onClose) =>
      SideEffects((({send}) => Animate(0., onClose) |> send))
    | BeginClosing(onBeginClosing, onClose) =>
      SideEffects(
        (
          ({send}) => {
            switch (onBeginClosing) {
            | None => ()
            | Some(f) => f()
            };
            send(Animate(0., onClose));
          }
        ),
      )
    | Open(onOpen) =>
      SideEffects((({send}) => Animate(targetHeight, onOpen) |> send))
    },
  willUnmount: ({state}) => state.animation |> SpringAnimation.stop,
  render: ({state}) =>
    <div
      style=(
        ReactDOMRe.Style.make(
          ~height=string_of_float(state.height) ++ "px",
          ~overflow="hidden",
          (),
        )
      )>
      (children |> ReasonReact.array)
    </div>,
};