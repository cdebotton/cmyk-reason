let component = ReasonReact.statelessComponent("Loader");

let make = _children => {
  ...component,
  render: _self => <p> {"Render a spinner" |> ReasonReact.string} </p>,
};