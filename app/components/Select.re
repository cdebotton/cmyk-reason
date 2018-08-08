module Styles = {
  let container = Css.(style([position(relative)]));
  let label = Css.(style([cursor(`pointer)]));
  let dropdown =
    Css.(
      style([
        position(absolute),
        listStyleType(none),
        top(100. |. pct),
        margin(0 |. px),
        padding(0 |. px),
      ])
    );
  let item = Css.(style([cursor(`pointer)]));
};

type state =
  | Closed
  | Open;

type action =
  | Toggle(state);

let component = ReasonReact.reducerComponent("Select");

type selectOption = {
  value: string,
  label: string,
};

let rec getLabel = (value, options) =>
  switch (options) {
  | [option, ..._options] when option.value === value => option.label
  | [_option, ...options] => options |> getLabel(value)
  | [] => Js.Exn.raiseError({j|$value doesn't exist in options|j})
  };

let make =
    (
      ~options: list(selectOption),
      ~value=?,
      ~onChange,
      ~placeholder,
      _children,
    ) => {
  ...component,
  initialState: () => Closed,
  reducer: (action, _state) =>
    switch (action) {
    | Toggle(state) => Update(state)
    },
  render: ({send, state}) => {
    let label =
      switch (value) {
      | Some(raw) => options |> getLabel(raw)
      | None => placeholder
      };

    let toggle = _event =>
      switch (state) {
      | Open => send(Toggle(Closed))
      | Closed => send(Toggle(Open))
      };

    <div className=Styles.container>
      <span className=Styles.label onClick=toggle>
        (label |> ReasonReact.string)
      </span>
      (
        switch (state) {
        | Open =>
          <ul className=Styles.dropdown>
            (
              options
              |> List.map(({label, value}) => {
                   let onClick = _event => {
                     onChange(value);
                     send(Toggle(Closed));
                   };
                   <li key={j|OPTION_$value|j} onClick className=Styles.item>
                     (label |> ReasonReact.string)
                   </li>;
                 })
              |> Array.of_list
              |> ReasonReact.array
            )
          </ul>
        | Closed => ReasonReact.null
        }
      )
    </div>;
  },
};