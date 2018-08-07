module type Configuration = {
  type t;
  type key;
  type value;
  let debugName: string;
  let get: (key, t) => value;
  let set: ((key, value), t) => t;
};

type interface('key, 'value) = {
  getValue: 'key => 'value,
  onChange: ('key, 'value) => unit,
  handleSubmit: ReactEvent.Form.t => unit,
};

module Make = (Form: Configuration) => {
  type state = {
    data: Form.t,
    dirty: bool,
  };
  type action =
    | Change((Form.key, Form.value))
    | Submit
    | Reset;

  let component = ReasonReact.reducerComponent(Form.debugName);

  let make = (~initialValues, ~onSubmit, children) => {
    let pristineState = ref(initialValues);

    {
      ...component,
      initialState: () => {data: initialValues, dirty: false},
      reducer: (action, state) =>
        switch (action) {
        | Change((key, value)) =>
          let data = state.data |> Form.set((key, value));
          ReasonReact.Update({dirty: true, data});
        | Reset => ReasonReact.Update({data: pristineState^, dirty: false})
        | Submit =>
          pristineState := state.data;
          ReasonReact.Update({...state, dirty: false});
        },
      render: ({send, state}) => {
        let onChange = (key, value) => Change((key, value)) |> send;
        let getValue = key => state.data |> Form.get(key);
        let handleSubmit = event => {
          event |> ReactEvent.Form.preventDefault;
          onSubmit(state.data);
        };

        children({onChange, getValue, handleSubmit});
      },
    };
  };
};