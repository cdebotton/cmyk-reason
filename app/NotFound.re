let component = ReasonReact.statelessComponent("NotFound");

let make = _children => {
  ...component,
  render: _self =>
    <Status code=404> ("Not found" |> ReasonReact.string) </Status>,
};