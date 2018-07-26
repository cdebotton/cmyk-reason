let component = ReasonReact.statelessComponent("Redirect");

let make = (~to_, _children) => {
  ...component,
  didMount: _self => ReasonReact.Router.push(to_) |> ignore,
  render: _self => ReasonReact.null,
};