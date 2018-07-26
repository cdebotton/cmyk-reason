module type Configuration = {
  type t;
  type key;
  type value;
  let debugName: string;
  let initialValues: t;
  let get: (key, t) => value;
  let set: ((key, value), t) => t;
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
  type interface = {
    getValue: Form.key => Form.value,
    onChange: (Form.key, Form.value) => unit,
  };

  let component = ReasonReact.reducerComponent(Form.debugName);

  let make = children => {
    let pristineState = ref(Form.initialValues);

    {
      ...component,
      initialState: () => {data: Form.initialValues, dirty: false},
      reducer: (action, state) =>
        switch (action) {
        | Change((key, value)) =>
          ReasonReact.Update({
            dirty: true,
            data: state.data |> Form.set((key, value)),
          })
        | Reset => ReasonReact.Update({data: pristineState^, dirty: false})
        | Submit =>
          pristineState := state.data;
          ReasonReact.Update({...state, dirty: false});
        },
      render: ({send, state}) => {
        let onChange = (key, value) => Change((key, value)) |> send;
        let getValue = key => state.data |> Form.get(key);

        children({onChange, getValue});
      },
    };
  };
};