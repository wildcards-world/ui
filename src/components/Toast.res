// @react.component
// let make = () => {
//   let toast = React.useContext(ToastProvider.ToastContext.context)
//   let toastDispatch = React.useContext(ToastProvider.DispatchToastContext.context)

//   React.useEffect1(() => {
//     let timeout = Js.Global.setTimeout(() => {
//       toastDispatch(Hide)
//     }, 5000)
//     Some(() => Js.Global.clearTimeout(timeout))
//   }, [toast])

//   let (toastHeading, toastInfo, _type) = toast

//   let color = switch _type {
//   | Error => "red"
//   | Warning => "yellow"
//   | Success => "green"
//   | Info => "blue"
//   }

//   <div
//     onClick={_ => toastDispatch(Hide)}
//     className="fixed bottom-3 flex flex-col"
//     style={ReactDOM.Style.make(~display=toastHeading->String.length > 0 ? "block" : "none", ())}>
//     <div
//       className={`flex flex-row items-center text-xl rounded-sm bg-white my-4 mx-10 py-2 px-4 border border-${color}-400 border-opacity-30`}>
//       <div
//         className={`animate-ping inline-flex h-3 w-3 mr-4 rounded-full bg-${color}-400 opacity-90`}
//       />
//       <div>
//         <div className="text-sm"> {toastHeading->React.string} </div>
//         <div className="text-xs"> {toastInfo->React.string} </div>
//       </div>
//     </div>
//   </div>
// }

