module Styles = {
  open Css;

  let modal = style([position(absolute), top(0 |> px), left(0 |> px)]);
};

type state =
  | Opened
  | Closed;

type action =
  | Open
  | Close;

type reactRef = Js.nullable(ReasonReact.reactRef) => unit;

[@bs.module "react"] external createRef: unit => reactRef = "";
[@bs.get] external current: reactRef => Dom.element = "";
let component = ReasonReact.reducerComponent("Popover");

let make = children => {
  let buttonRef = createRef();

  {
    ...component,
    initialState: () => Closed,
    reducer: (action, _state) =>
      switch (action) {
      | Open => Update(Opened)
      | Close => Update(Closed)
      },
    render: ({state, send}) => {
      let onClick = _event =>
        switch (state) {
        | Opened => send(Close)
        | Closed => send(Open)
        };
      let portal =
        switch (ReactDOMRe._getElementById("portal")) {
        | exception _exn => None
        | None => None
        | Some(element) => Some(element)
        };

      let test = buttonRef->current;
      Js.log(test);

      <Fragment>
        <Button ref=buttonRef onClick> {"Open" |> ReasonReact.string} </Button>
        {
          switch (state, portal) {
          | (Opened, Some(portal)) =>
            ReactDOMRe.createPortal(
              <div className=Styles.modal> children </div>,
              portal,
            )
          | (_, _) => ReasonReact.null
          }
        }
      </Fragment>;
    },
  };
};