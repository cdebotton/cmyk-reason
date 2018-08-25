module Styles = {
  let colors = Colors.neutral |> Utils.joinList;

  let container =
    Css.(
      style([
        position(relative),
        display(`flex),
        userSelect(none),
        before([
          `declaration(("content", " ")),
          position(absolute),
          display(block),
          top(100. |> pct),
          width(100. |> pct),
          height(2 |> px),
          `declaration((
            "backgroundImage",
            {j|linear-gradient(to right, $colors)|j},
          )),
        ]),
      ])
    );

  let currentValue =
    Css.(
      style([
        display(`flex),
        flexDirection(`row),
        flexWrap(`nowrap),
        justifyContent(`spaceBetween),
        cursor(`pointer),
        fontSize(1. |> rem),
        width(100. |> pct),
        padding2(~v=0.5 |> rem, ~h=0.75 |> rem),
        border(0 |> px, none, transparent),
        focus([outline(0 |> px, none, transparent)]),
      ])
    );

  let label =
    Css.(
      style([
        position(absolute),
        pointerEvents(none),
        userSelect(none),
        fontSize(0.6 |> rem),
        fontWeight(600),
        textTransform(uppercase),
        left(0.75 |> rem),
        opacity(0.),
      ])
    );

  let dropdown =
    Css.(
      style([
        position(absolute),
        listStyleType(none),
        backgroundColor(white),
        paddingTop(0.5 |> rem),
        top(100. |> pct),
        margin(0 |> px),
        padding(0 |> px),
        width(100. |> pct),
        borderRadius(5 |> px),
        overflow(hidden),
        zIndex(100),
        boxShadow(
          ~x=0 |> px,
          ~y=2 |> px,
          ~blur=4 |> px,
          rgba(0, 0, 0, 0.25),
        ),
      ])
    );

  let item =
    Css.(
      style([
        cursor(`pointer),
        fontSize(1. |> rem),
        padding2(~v=0.5 |> rem, ~h=0.75 |> rem),
        hover([backgroundColor(hsla(200, 25, 25, 0.05))]),
      ])
    );
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
      ~className=?,
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
      | None => ""
      };

    let selections =
      options
      |> List.map(({label, value}) => {
           let onClick = _event => {
             onChange(value);
             send(Toggle(Closed));
           };
           <li key={j|OPTION_$value|j} onClick className=Styles.item>
             {label |> ReasonReact.string}
           </li>;
         })
      |> Array.of_list;

    let toggle = _event =>
      switch (state) {
      | Open => send(Toggle(Closed))
      | Closed => send(Toggle(Open))
      };

    <div
      className={
        [Some(Styles.container), className]
        |> Utils.unwrapOptionalList
        |> Utils.joinList(~sep=" ")
      }>
      <Label
        className=Styles.label
        pose={
          switch (value) {
          | None => Hidden
          | _ => Visible
          }
        }>
        {placeholder |> ReasonReact.string}
      </Label>
      <span className=Styles.currentValue onClick=toggle>
        {label |> ReasonReact.string}
        {
          let icon =
            switch (state) {
            | Open => SolidIcons.faChevronUp
            | Closed => SolidIcons.faChevronDown
            };
          <FontAwesomeIcon icon size=ExtraSmall />;
        }
      </span>
      {
        switch (state) {
        | Open => <ul className=Styles.dropdown> ...selections </ul>
        | Closed => ReasonReact.null
        }
      }
    </div>;
  },
};