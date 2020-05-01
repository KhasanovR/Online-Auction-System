-- MySQL dump 10.13  Distrib 5.7.24, for Linux (x86_64)
--
-- Host: localhost    Database: auction
-- ------------------------------------------------------
-- Server version	5.7.24-0ubuntu0.18.10.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `bid`
--

DROP TABLE IF EXISTS `bid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bid` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `lot_id` int(11) DEFAULT NULL,
  `bid_amount` decimal(12,2) DEFAULT NULL,
  `bidder_client_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `lot_id` (`lot_id`),
  KEY `bidder_client_id` (`bidder_client_id`),
  CONSTRAINT `bid_ibfk_1` FOREIGN KEY (`lot_id`) REFERENCES `lot` (`id`),
  CONSTRAINT `bid_ibfk_2` FOREIGN KEY (`bidder_client_id`) REFERENCES `client` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bid`
--

LOCK TABLES `bid` WRITE;
/*!40000 ALTER TABLE `bid` DISABLE KEYS */;
/*!40000 ALTER TABLE `bid` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `client`
--

DROP TABLE IF EXISTS `client`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) DEFAULT NULL,
  `password` varchar(64) DEFAULT NULL,
  `visa_card_num` varchar(19) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`),
  KEY `visa_card_num` (`visa_card_num`),
  CONSTRAINT `client_ibfk_1` FOREIGN KEY (`visa_card_num`) REFERENCES `visa_card` (`visa_card_num`)
) ENGINE=InnoDB AUTO_INCREMENT=67 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `client`
--

LOCK TABLES `client` WRITE;
/*!40000 ALTER TABLE `client` DISABLE KEYS */;
INSERT INTO `client` VALUES (46,'U1610176','abcac824a27d367a74fc184f095fc1a8f6d62527','8323474106275695891'),(47,'U1610064','36fca40997c9d4d906f89bebb6e82e5ef92fbffa','0516339722149745868'),(48,'U1610026','f7c3bc1d808e04732adf679965ccc34ca7ae3441','5980512843162973647'),(49,'U1610249','6a14c1f1ac2f6e39a76818df47957bb62e8ed336','7658510894392123674'),(50,'U1610183','ff5098b0f95ee4637ce0cb8a31df3af260704872','3847153417295860296'),(51,'U1610054','8047695e50f9fcbbab1058fc30775714e2809db8','8323474106275695891'),(52,'U1610072','cd15b3ef37acc6a61d03bc5edf8f83d4399131c5','8323474106275695891'),(53,'U1610041','165b6d58772be29861ba0eeb00e3ed01c35c1051','5712493946362708851'),(54,'U1610060','63c86665f3bcdb1360c9901ce039d9bb863da19e','3357169241526940788'),(55,'U1610250','1a0ff74ac3b4df374dd1d7b19653031f2b258d1f','3357169241526940788'),(56,'U1610202','f7bcd41476c43ae675fe0bba6b9fa2cf361a5da4','0516339722149745868'),(57,'U1610047','1165c91acfc3775fc987203f1f0bb5681c90e498','0516339722149745868'),(58,'U1610205','e5446252b6d876734251af32e20d1436b68e68a8','5980512843162973647'),(59,'U1610016','006688169aeeb7c704bf377a5f42cc3c92eda71a','8323474106275695891'),(60,'U1610127','6597d69b7c68f492b66de4f5e0ab505c9152e17a','8323474106275695891'),(61,'U1610130','f41555deb9f46ce7132c3526cdb8ff5cddf7009d','3847153417295860296'),(62,'U1610211','ddc515deeaf651450c1641c26f2a4eae7e325e55','3847153417295860296'),(63,'U1610052','a9541e35ff33f458b68d6e7d0ae2605202d926cd','8323474106275695891'),(64,'U1610061','4d30471168148f76b258c76b057752e6e9e01f2e','8323474106275695891'),(65,'U1610040deleted','deletedcafaa838ee0cfb27d30771a1498f9706',NULL),(66,'U1610040','e60af00cf06365381b042b7a40fca6c580b40df3','5980512843162973647');
/*!40000 ALTER TABLE `client` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `lot`
--

DROP TABLE IF EXISTS `lot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lot` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `min_price` decimal(12,2) DEFAULT NULL,
  `winning_bid` decimal(12,2) DEFAULT NULL,
  `winning_bidder` int(11) DEFAULT NULL,
  `title` varchar(255) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `owner_id` int(11) DEFAULT NULL,
  `start_time` datetime DEFAULT NULL,
  `stop_time` datetime DEFAULT NULL,
  `transaction_done` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `winning_bidder` (`winning_bidder`),
  KEY `owner_id` (`owner_id`),
  CONSTRAINT `lot_ibfk_1` FOREIGN KEY (`winning_bidder`) REFERENCES `client` (`id`),
  CONSTRAINT `lot_ibfk_2` FOREIGN KEY (`owner_id`) REFERENCES `client` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=52 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lot`
--

LOCK TABLES `lot` WRITE;
/*!40000 ALTER TABLE `lot` DISABLE KEYS */;
INSERT INTO `lot` VALUES (49,5.25,NULL,NULL,'VASE','An ancient vase from GREEK church really precious artifact.',46,'2018-12-21 18:41:48','2018-12-21 18:46:48',0),(50,12.50,NULL,NULL,'BOOK','Harry Potter and Chambers of Secrets',49,'2018-12-21 18:46:48','2018-12-21 18:51:48',0),(51,12.50,NULL,NULL,'BOOK','Harry Potter and Chambers of Secrets',49,'2018-12-21 18:51:48','2018-12-21 18:56:48',0);
/*!40000 ALTER TABLE `lot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `visa_card`
--

DROP TABLE IF EXISTS `visa_card`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `visa_card` (
  `visa_card_num` varchar(19) NOT NULL,
  `balance` decimal(12,2) DEFAULT NULL,
  PRIMARY KEY (`visa_card_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `visa_card`
--

LOCK TABLES `visa_card` WRITE;
/*!40000 ALTER TABLE `visa_card` DISABLE KEYS */;
INSERT INTO `visa_card` VALUES ('0123456789123456789',12345.25),('0516339722149745868',5375.14),('3357169241526940788',5204.22),('3847153417295860296',8125.44),('5712493946362708851',7391.66),('5980512843162973647',5000.00),('7632195706154864328',5406.03),('7658510894392123674',57129.60),('8323474106275695891',29000.60),('9538891466257703241',2238.98);
/*!40000 ALTER TABLE `visa_card` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-12-21 18:10:05
