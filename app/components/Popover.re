module Styles = {
  open Css;

  let modal = (~size as (w, h), ~position as (x, y)) =>
    style([
      position(absolute),
      backgroundColor(white),
      width(px(w)),
      height(px(h)),
      borderRadius(px(3)),
      left(px(x)),
      top(px(y)),
      overflowY(auto),
      boxShadow(
        ~x=rem(0.125),
        ~y=rem(0.125),
        ~blur=rem(0.5),
        hsla(0, 0, 0, 0.5),
      ),
    ]);

  let bgColor = Colors.getColor(Neutral);
  let fgColor = Colors.getColor(Light);
  let buttonBackgroundColors = [bgColor(0), bgColor(3)] |> Utils.joinList;
  let textColor = fgColor(1);
  let heading =
    style([
      color(white),
      margin(px(0)),
      width(pct(100.)),
      height(rem(3.)),
      display(`flex),
      flexDirection(row),
      flexWrap(nowrap),
      alignItems(center),
      padding2(~v=rem(0.), ~h=rem(1.)),
      `declaration((
        "backgroundImage",
        {j|linear-gradient(to bottom right, $buttonBackgroundColors)|j},
      )),
    ]);
};

type state =
  | Opened
  | Closed;

type action =
  | Open
  | Close;

let component = ReasonReact.reducerComponent("Popover");

type position = (float, float);

type size = {
  width: int,
  height: int,
};

let getModalPosition = (~size as (w, h), ~distance, button) =>
  DomApi.(
    switch (window, document) {
    | exception _ => None
    | (Some(window), Some(document)) =>
      let distancef = float_of_int(distance);
      let heightf = float_of_int(h);
      let widthf = float_of_int(w);

      let distanceToBottom = button->bottomGet +. distancef +. heightf;
      let distanceToTop = button->topGet -. distancef -. heightf;
      let distanceToRight = button->rightGet +. distancef +. widthf;
      let distanceToLeft = button->leftGet -. distancef -. widthf;

      let viewportBottom = Window.(window->pageYOffset +. window->innerHeight);
      let viewportTop = Window.(window->pageYOffset);
      let viewportRight = Window.(window->pageXOffset +. window->innerWidth);
      let viewportLeft = Window.(window->pageXOffset);

      let fitsBelow = viewportBottom >= distanceToBottom;
      let fitsAbove = distanceToTop >= viewportTop;
      let fitsToLeft = distanceToLeft >= viewportLeft;
      let fitsToRight = viewportRight >= distanceToRight;

      let minPositionX = distance;
      let maxPositionX =
        (Body.(document->body->offsetWidth) |> int_of_float) - distance;
      let minPositionY = distance;
      let maxPositionY =
        (Body.(document->body->offsetHeight) |> int_of_float) - distance;

      let x =
        switch (fitsAbove, fitsToRight, fitsBelow, fitsToLeft) {
        | (_, _, true, _)
        | (true, _, _, _) =>
          int_of_float(
            button->leftGet +. button->widthGet /. 2. -. widthf /. 2.,
          )
        | (_, true, _, _) => int_of_float(button->rightGet +. distancef)
        | (_, _, _, true) =>
          int_of_float(button->leftGet -. distancef -. widthf)
        | (false, false, false, false) => 0
        };

      let y =
        switch (fitsAbove, fitsToRight, fitsBelow, fitsToLeft) {
        | (_, _, true, _) => int_of_float(button->bottomGet +. distancef)
        | (true, _, _, _) => int_of_float(distanceToTop)
        | (_, true, _, _)
        | (_, _, _, true) =>
          int_of_float(
            button->topGet +. button->heightGet /. 2. -. heightf /. 2.,
          )
        | (false, false, false, false) => 0
        };

      let breaksTop = y < minPositionY;
      let breaksRight = x + w / 2 > maxPositionX;
      let breaksBottom = y + h / 2 > maxPositionY;
      let breaksLeft = x < minPositionX;

      let x =
        switch (breaksLeft, breaksRight) {
        | (true, true)
        | (false, true) => maxPositionX - w
        | (true, false) => minPositionY
        | (false, false) => x
        };

      let y =
        switch (breaksTop, breaksBottom) {
        | (true, true)
        | (true, false) => minPositionY
        | (false, true) => maxPositionY - h
        | (false, false) => y
        };

      Some((x, y));
    | _ => None
    }
  );

type interface = {
  open_: unit => unit,
  close: unit => unit,
};

let make = (~title=?, ~label, ~size=(600, 400), ~distance=12, children) => {
  let buttonRef: ref(option(Dom.element)) = ref(None);

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

      <Fragment>
        <Button innerRef={c => buttonRef := c |> Js.Nullable.toOption} onClick>
          {label |> ReasonReact.string}
        </Button>
        {
          switch (buttonRef^) {
          | exception _ => ReasonReact.null
          | None => ReasonReact.null
          | Some(button) =>
            let position =
              DomApi.(button->getBoundingClientRect)
              |> getModalPosition(~size, ~distance);

            switch (state, portal, position) {
            | (Opened, Some(portal), Some(position)) =>
              let open_ = () => Open |> send;
              let close = () => Close |> send;
              ReactDOMRe.createPortal(
                <div className={Styles.modal(~size, ~position)}>
                  {
                    switch (title) {
                    | None => ReasonReact.null
                    | Some(title) =>
                      <Heading className=Styles.heading level=3>
                        {title |> ReasonReact.string}
                      </Heading>
                    }
                  }
                  {children({open_, close})}
                </div>,
                portal,
              );
            | _ => ReasonReact.null
            };
          }
        }
      </Fragment>;
    },
  };
};