type state =
  | Open
  | Closed;

type action =
  | Toggle(state);

let component = ReasonReact.reducerComponent("Popover");

let make = (~label, children) => {
  let buttonRef: ref(Js.nullable(ReasonReact.reactRef)) =
    ref(Js.Nullable.null);
  {
    ...component,
    initialState: () => Closed,
    didMount: _self =>
      switch (Js.Nullable.toOption(buttonRef^)) {
      | Some(button) =>
        let element = button |> ReasonReact.refToJsObj;
        element |> Js.log;
      | None => ()
      },
    reducer: (action, _state) =>
      switch (action) {
      | Toggle(state) => Update(state)
      },
    render: ({send, state}) => {
      let onClick = _event =>
        switch (state) {
        | Open => send(Toggle(Closed))
        | Closed => send(Toggle(Open))
        };

      let popover =
        switch (state) {
        | Closed => ReasonReact.null
        | Open => <div> ...children </div>
        };

      <Fragment>
        <Button ref=(c => buttonRef := c) format=Neutral onClick>
          (label |> ReasonReact.string)
        </Button>
        popover
      </Fragment>;
    },
  };
};