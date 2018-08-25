let component = ReasonReact.statelessComponent("AdminCreateUser");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <Heading level=4> ("Create a new user" |> ReasonReact.string) </Heading>
    </div>,
};
