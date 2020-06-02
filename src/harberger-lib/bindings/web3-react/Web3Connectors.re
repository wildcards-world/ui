type injectedType = {isAuthorized: unit => Promise.promise(bool)};

[@bs.module "./connectors"] external injected: injectedType = "injected";
