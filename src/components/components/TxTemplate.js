import React, { Fragment } from 'react';
import { Heading, Text, Loader } from 'rimble-ui'
import { centerItems } from '../../Styles.bs'
import WildcardsLoader from '../StaticContent/WildcardsLoader'

export default ({ txObjects, children }) => txObjects.length > 0 ?
  <div style={{ margin: "1em" }}>
    <Heading.h3>Processing Transaction</Heading.h3>
    {
      !!txObjects[0] ?
        <Fragment>
          {(() => {
            switch (txObjects[0].status) {
              case 'pending':
                return <Fragment>
                  {/* <Text>The transaction was is being processed.</Text> */}
                  <Loader className={centerItems} color='green' size='80px' />
                </Fragment>
              case 'error':
                return <Text>The transaction was failed, please try again.</Text>
              case 'success':
                return <Fragment>
                  <Text>The transaction was successful.</Text>
                  <Text>It has been confirmed in the blockchain{/*txObjects[0].confirmations.length*/}.</Text>
                  <Text>
                    <a href={'https://etherscan.io/tx/' + txObjects[0].receipt.transactionHash} target='_blank'>View transaction on Ethersan</a>
                  </Text>
                </Fragment>
            }
          })()}
        </Fragment>
        :
        <Fragment>
          <Text>Sending transaction to signer.</Text>
          <Loader className={centerItems} color='green' size='80px' />
          <WildcardsLoader/>
        </Fragment>
    }
  </div >
  :
  <Fragment>
    {children}
  </Fragment>
