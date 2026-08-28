# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Fixed
- MdnsServer::defaultName static initialization order dependency.
- MdnsServer being used as its own event handler, which could be called during or after its destruction.
- MdnsServer::HandleEvent calling into the event source server while holding the mDNS server lock, inverting the lock order used everywhere else.

## [1.2.4] - 2026-08-27
### Fixed
- MdnsServer::SetHostname restarting the server even when the hostname did not change.
- MdnsServer::RemoveService restarting the server even when no service was removed.

## [1.2.3] - 2026-08-26
### Fixed
- mdns_server example and test server not stopping the server task before destruction.
- MdnsServer destruction sequence.
- Lock order inversion in MdnsServer::AddService.
- MdnsServer services staying advertised after Disable or a failed Enable when the shared mDNS refcount does not reach zero.

## [1.2.2] - 2026-08-19
### Fixed
- DnsSdQuery documentation referencing a nonexistent timeout parameter.

## [1.2.1] - 2026-08-14
### Fixed
- DnsSdQuery null TXT record key or value handling.
- Read timeout conversion to milliseconds
- DnsSdQuery IPv6 address instantiation.

## [1.2.0] - 2026-08-11
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