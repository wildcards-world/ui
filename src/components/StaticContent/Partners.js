import React from "react"
import { Flex, Box, Card, Heading, Text } from 'rimble-ui';
import { infoBackground } from '../../Styles.bs'
import UbisoftLogo from '../../img/logos/Ubisoft.png'
import EthCapeTownLogo from '../../img/logos/EthCapeTown.png'
import WildTomorrowFundLogo from '../../img/logos/WildTomorrowFund.png'

const styles = {
  logo: {
    marginLeft: '10%',
    width: '80%'
  }
}

const Partners = () => <div>
  <Flex className={infoBackground} px={50}>
    <Box m={50} width={1 / 3} color="black">
      <Card>        
        <img src={UbisoftLogo} style={styles.logo}/>         
        <Text>
          Incubator program
        </Text>
      </Card>
    </Box>
    <Box m={50} width={1 / 3} color="black">      
      <Card>        
        <img src={EthCapeTownLogo} style={styles.logo}/>         
        <Text>
          Hackathon winners
        </Text>
      </Card>
    </Box>
    <Box m={50} width={1 / 3} color="black">
      <Card>        
        <img src={WildTomorrowFundLogo} style={styles.logo}/>         
        <Text>
          Wildlife conservation partner
        </Text>
      </Card>      
    </Box>
  </Flex>
</div>

export default Partners