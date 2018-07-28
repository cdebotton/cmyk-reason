type t('action) = {mutable send: 'action => unit};

type token('action) = t('action);

let sendDefault = _action => ();

let create = () => {send: sendDefault};

let subscribe = (~send, x) =>
  if (x.send === sendDefault) {
    x.send = send;
    Some(x);
  } else {
    None;
  };

let unsubscribe = x => x.send = sendDefault;

let send = (x, ~action) => x.send(action);