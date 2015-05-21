/*
Navicat MySQL Data Transfer

Source Server         : qq
Source Server Version : 50087
Source Host           : localhost:3306
Source Database       : dev_monitor

Target Server Type    : MYSQL
Target Server Version : 50087
File Encoding         : 65001

Date: 2015-05-05 09:57:41
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for tb_dev_corrected_value_conf
-- ----------------------------
DROP TABLE IF EXISTS `tb_dev_corrected_value_conf`;
CREATE TABLE `tb_dev_corrected_value_conf` (
  `id` int(11) NOT NULL auto_increment,
  `cinema_name` varchar(200) default NULL,
  `cinema_hall` varchar(200) default NULL,
  `dev_ip` varchar(32) default NULL,
  `fix_ftl` float(15,5) default NULL,
  `fix_lc` float(15,5) default NULL COMMENT '声压左声道',
  `fix_rc` float(15,5) default NULL COMMENT '声压右声道',
  `fix_cc` float(15,5) default NULL COMMENT '声压中置',
  `fix_hb` float(15,5) default NULL COMMENT '声压重低音',
  `fix_la` float(15,5) default NULL COMMENT '声压左环绕音效',
  `fix_ra` float(15,5) default NULL COMMENT '声压右环绕音效',
  `start_datetime` time default NULL COMMENT '开始检测日期',
  `end_datetime` time default NULL COMMENT '结束简测时间',
  `script` varchar(400) default NULL,
  `sync_mode` int(3) default NULL,
  `port` int(7) default NULL,
  `atmost` varchar(6) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_dev_corrected_value_conf
-- ----------------------------
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('4', '金逸中关村', '1号厅', '192.168.56.151', '0.00000', '10.00000', '10.00000', '9.00000', '4.00000', '10.00000', '10.00000', '09:00:00', '10:30:00', null, '0', '10001', 'no');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('5', '金逸中关村', '2号厅', '192.168.56.152', '0.00000', '10.00000', '8.00000', '12.00000', '2.00000', '6.00000', '1.00000', '09:00:00', '10:30:00', null, '0', '10001', 'no');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('6', '金逸中关村', '3号厅', '192.168.56.153', '0.00000', '10.00000', '10.00000', '12.00000', '15.00000', '10.00000', '10.00000', '09:00:00', '10:30:00', null, '0', '10001', 'no');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('7', '金逸中关村', '4号厅', '192.168.56.154', '0.00000', '15.00000', '15.00000', '12.00000', '15.00000', '12.00000', '12.00000', '09:00:00', '10:30:00', null, '0', '10001', 'no');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('8', '金逸中关村', '5号厅', '192.168.56.155', '0.00000', '10.00000', '8.00000', '10.00000', '0.00000', '0.00000', '-3.00000', '09:00:00', '10:30:00', null, '0', '10001', 'yes');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('9', '金逸中关村', '6号厅', '192.168.56.156', '0.00000', '16.00000', '16.00000', '18.00000', '16.00000', '12.00000', '12.00000', '09:00:00', '10:30:00', null, '0', '10001', 'no');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('10', '金逸中关村', '7号厅', '192.168.56.157', '0.00000', '10.00000', '10.00000', '12.00000', '15.00000', '14.00000', '10.00000', '09:00:00', '10:30:00', '', '0', '10001', 'no');
