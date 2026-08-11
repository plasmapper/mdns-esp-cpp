# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Test suite.

### Changed
- Lock timeout handling.
- Static const members to constexpr.

### Fixed
- Null hostname check in MdnsClient::DnsSdQuery.
- Null instance name check in MdnsClient::DnsSdQuery.
- Instance name filter bypass on empty discovered name in MdnsClient::DnsSdQuery.
- Removing event handler in MdnsServer::RemoveService.
- Invalid iterator use in MdnsServer::RemoveService.
- Uncorrelated mdns_init and mdns_free calls in MdnsServer and MdnsClient.
- Enabled state reported before service registration completed in MdnsServer::Enable.
- Default read timeout referencing NetworkStream instead of MdnsClient in MdnsClient::readTimeout.

## [1.1.0] - 2024-08-26
### Changed
- ESP-IDF dependency to 5.3 and mDNS dependency to 1.4.0.

## [1.0.1] - 2024-06-12
### Added
- Copying examples to component folder on upload.

## [1.0.0] - 2024-06-12
Initial release.