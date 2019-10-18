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
  <Flex flexWrap={"wrap"}
    alignItems={"center"} className={infoBackground} px={50}>
    <Box m={50} width={[1, 1, 0.265]} color="black">
      <Card>
        <a
          href="https://www.ubisoft.com/en-us/company/start-ups/station-f.aspx">
          <img src={UbisoftLogo} style={styles.logo} />
          <Text>
            Ubisoft's Entrepreneurs Lab, Season 4, participants
          </Text>
        </a>
      </Card>
    </Box>
    <Box m={50} width={[1, 1, 0.265]} color="black">
      <Card>
        <a
          href="https://ethcapetown.com/">
          <img src={EthCapeTownLogo} style={styles.logo} />
        </a>
        <Text>
          Overall winners of EthCapeTown hackathon
        </Text>
      </Card>
    </Box>
    <Box m={50} width={[1, 1, 0.265]} color="black">
      <Card>
        <a
          href="https://wildtomorrowfund.org">
          <img src={WildTomorrowFundLogo} style={styles.logo} />
        </a>
        <Text>
          Wild Tomorrow Fund, our wildlife conservation partner
        </Text>
      </Card>
    </Box>
  </Flex>
</div>

export default Partners
