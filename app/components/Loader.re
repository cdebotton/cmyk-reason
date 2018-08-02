let component = ReasonReact.statelessComponent("Loader");

let make = _children => {
  ...component,
  render: _self => <p> ("Loading" |> ReasonReact.string) </p>,
};