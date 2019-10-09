import React from "react"
import { Flex, Box, Card, Heading, Text } from 'rimble-ui';
import { infoBackground } from '../../Styles.bs'

const Partners = () => <div>
  <Flex styles={infoBackground}>
    <Box p={3} width={1 / 3} color="black">
      <Card>
        
        <Heading>Ubisoft</Heading>
        <Text>
          Incubator program
        </Text>
      </Card>
    </Box>
    <Box p={3} width={1 / 3} color="black">
      <Card>EthCapeTown</Card>

    </Box>
    <Box p={3} width={1 / 3} color="black">
      <Card>Wild Tommorrow Fund</Card>
    </Box>
  </Flex>
</div>

export default Partners