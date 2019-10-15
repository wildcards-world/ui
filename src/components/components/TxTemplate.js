import React, { Fragment, useState } from 'react';
import { Button, Modal, Card, Box, Heading, Text, Flex, Loader, Input, Radio } from 'rimble-ui'

export default ({ txObjects, children }) => txObjects.length > 0 ?
  <Fragment>
    <Heading.h3>Processing Transaction</Heading.h3>
    {
      !!txObjects[0] ?
        <Fragment>
          {(() => {
            switch (txObjects[0].status) {
              case 'pending':
                return <Fragment>
                  <Text>The transaction was is being processed.</Text>
                  <Loader color='red' size='80px' />
                </Fragment>
              case 'error':
                return <Text>The transaction was failed, please try again.</Text>
              case 'success':
                return <Fragment>
                  <Text>The transaction was successful.</Text>
                  <Text>It has been confirmed {txObjects[0].confirmations.length}.</Text>
                  <Text>
                    <a href={'https://etherscan.io/tx/' + txObjects[0].receipt.txHashtransactionHash} target='_blank'>View transaction on Ethersan</a>
                  </Text>
                </Fragment>
            }
          })()}
        </Fragment>
        :
        <Fragment>
          <Text>Sending transaction to signer.</Text>
          <Loader color='red' size='80px' />
        </Fragment>
    }
  </Fragment>
  :
  <Fragment>
    {children}
  </Fragment>
