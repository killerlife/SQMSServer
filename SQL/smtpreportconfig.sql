/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50087
Source Host           : localhost:3306
Source Database       : dev_monitor

Target Server Type    : MYSQL
Target Server Version : 50087
File Encoding         : 65001

Date: 2015-03-12 18:31:01
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for smtpreportconfig
-- ----------------------------
DROP TABLE IF EXISTS `smtpreportconfig`;
CREATE TABLE `smtpreportconfig` (
  `IndexId` int(11) NOT NULL,
  `SenderId` varchar(30) NOT NULL,
  `SenderPwd` varchar(30) NOT NULL,
  `SmtpServer` varchar(30) NOT NULL,
  `ServerPort` int(11) NOT NULL,
  `RecvList` varchar(500) NOT NULL,
  `SendTime` time NOT NULL,
  PRIMARY KEY  (`IndexId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of smtpreportconfig
-- ----------------------------
INSERT INTO `smtpreportconfig` VALUES ('1', 'S-QMS@leoniscinema.com', 'leonis62670467', 'smtp.qiye.163.com', '465', 'jaontolt@leoniscinema.com pc94@yeah.net\r\n', '13:55:04');
